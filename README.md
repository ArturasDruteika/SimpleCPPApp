# SimpleCPPApp: Debug C++ in Docker with IDE Breakpoints

This project demonstrates how to debug a C++ application running inside a Docker container, allowing you to hit breakpoints from your IDE (such as VS Code) on your host machine.

## How It Works

- The app is a simple C++17 program (see `main.cpp`).
- The Docker image builds the app in Debug mode and launches it under `gdbserver`.
- The container exposes port 2000 for remote debugging.
- The source code is mounted read-only into the container, while the build directory is mounted read-write (for incremental builds).
- The entrypoint script (`entrypoint.sh`) configures, builds, and launches the app under `gdbserver`.
- The solution now includes 4 shared libraries and 2 service executables that can run in separate containers.

## Prerequisites

- Docker installed on your host
- An IDE with C++ debugging support (e.g., VS Code with C++ extension)


## Quick Start

There are two main workflows:

### 1. Build on Host, Debug in Container

1. **Build the app on your host:**
   ```sh
   cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
   cmake --build build
   ```

2. **Build the Docker image:**
   ```sh
   docker build -t simplecppapp:debug .
   ```

3. **Run the container (using your host build):**
   ```sh
   docker run -it --rm \
     --name cppdbg \
     --cap-add=SYS_PTRACE \
     --security-opt seccomp=unconfined \
     -p 2000:2000 \
     -v "$PWD":/app:ro \
     -v "$PWD/build":/app/build:rw \
     -w /app \
     simplecppapp:debug "abcd"
   ```
   This will start the app under `gdbserver` listening on port 2000, using the binary you built on your host. You can replace `"abcd"` with any arguments you want to pass to the app.

---

### 2. Build and Debug Entirely in Container (Recommended for Clean/Consistent Builds)

1. **Build the Docker image:**
   ```sh
   docker build -t simplecppapp:debug .
   ```

2. **Run the container (container will build and debug):**
   
   #### Linux
   ```sh
   docker run -d --rm --name cppdbg --privileged -v "$PWD":/app -v cpp_build:/app/build -w /app --entrypoint sleep   simplecppapp:debug infinity
   ```

   #### Windows
   ```sh
   docker run -d --rm --name cppdbg --privileged -v ${PWD}:/app -v cpp_build:/app/build -w /app --entrypoint sleep simplecppapp:debug infinity
   ```
   The entrypoint script will build the app inside the container and launch it under `gdbserver`.



3. Exec into the container
    ```sh
    docker exec -it cppdbg bash
    tty
    ```

---

## Configure your IDE for Remote Debugging

Regardless of where you build, you can debug using your IDE (e.g., VS Code) by attaching to the running `gdbserver` in the container.

### VS Code Example (`launch.json`)

#### If you built on the host (binary at `${workspaceFolder}/build/SimpleCPPApp`):

```json
{
  "version": "0.2.0",
  "configurations": [
    {
      "name": "Attach to gdbserver in Docker (host build)",
      "type": "cppdbg",
      "request": "launch",
      "MIMode": "gdb",
      "miDebuggerPath": "gdb",
      "miDebuggerServerAddress": "localhost:2000",
      "program": "${workspaceFolder}/build/SimpleCPPApp",
      "cwd": "${workspaceFolder}",
      "stopAtEntry": false,
      "sourceFileMap": {
        "/app": "${workspaceFolder}"
      },
      "setupCommands": [
        { "text": "set auto-load safe-path /" },
        { "text": "-enable-pretty-printing" },
        {
          "text": "python import sys; sys.path.insert(0, '/usr/share/gcc/python'); import libstdcxx.v6.printers as p; p.register_libstdcxx_printers(None)"
        },
        { "text": "set print pretty on" },
        { "text": "set print object on" },
        { "text": "set print elements 0" }
      ]
    }
  ]
}
```

#### If you built in the container (binary at `/app/build/SimpleCPPApp`):

```json
{
  "version": "0.2.0",
  "configurations": [
    {
      "name": "Debug (gdb in container via docker exec)",
      "type": "cppdbg",
      "request": "launch",
      "MIMode": "gdb",

      "program": "/app/build/SimpleCPPApp",
      "args": ["abcd"],
      "cwd": "/app",

      "pipeTransport": {
        "pipeProgram": "docker",
        "pipeArgs": ["exec", "-i", "cppdbg", "sh", "-lc"],
        "debuggerPath": "/usr/bin/gdb"
      },

      "externalConsole": false,

      "sourceFileMap": {
        "/app": "${workspaceFolder}"
      },

      "setupCommands": [
        { "text": "set inferior-tty /dev/pts/0" },
        { "text": "set auto-load safe-path /" },
        { "text": "-enable-pretty-printing" },
        {
          "text": "python import sys; sys.path.insert(0, '/usr/share/gcc/python'); import libstdcxx.v6.printers as p; p.register_libstdcxx_printers(None)"
        },
        { "text": "set print pretty on" },
        { "text": "set print object on" },
        { "text": "set print elements 0" }
      ]
    }
  ]
}

```

**Note:** The only difference is where the binary was built. The path mapping remains the same as long as you mount your workspace to `/app`.

---

5. **Set breakpoints in your IDE and start debugging!**

## Services + Shared Libraries (new)

This project now has:
- 4 shared libs: `core`, `math`, `text`, `time`
- 2 service executables: `service1`, `service2`

The service binaries are built into `/app/build/service1` and `/app/build/service2`.

### Run services in separate containers

Use the new docker compose file to run each service in its own container and gdbserver port:

```sh
docker compose up --build
```

This will start:
- `service1` in container `cpp_service1` on port `2001`
- `service2` in container `cpp_service2` on port `2002`

### VS Code pipeTransport examples

Attach to service1:

```json
{
  "name": "Debug service1 (container)",
  "type": "cppdbg",
  "request": "launch",
  "MIMode": "gdb",
  "program": "/app/build/service1",
  "cwd": "/app",
  "pipeTransport": {
    "pipeProgram": "docker",
    "pipeArgs": ["exec", "-i", "cpp_service1", "sh", "-lc"],
    "debuggerPath": "/usr/bin/gdb"
  },
  "sourceFileMap": {
    "/app": "${workspaceFolder}"
  }
}
```

Attach to service2:

```json
{
  "name": "Debug service2 (container)",
  "type": "cppdbg",
  "request": "launch",
  "MIMode": "gdb",
  "program": "/app/build/service2",
  "cwd": "/app",
  "pipeTransport": {
    "pipeProgram": "docker",
    "pipeArgs": ["exec", "-i", "cpp_service2", "sh", "-lc"],
    "debuggerPath": "/usr/bin/gdb"
  },
  "sourceFileMap": {
    "/app": "${workspaceFolder}"
  }
}
```

## Notes
- You can pass arguments to the app by appending them to the `docker run` command.
- The container rebuilds the app on each start if sources change.
- The build system uses CMake and Ninja (or Makefiles if Ninja is unavailable).

## Files
- `main.cpp`: Example C++ app
- `Dockerfile`: Build environment and entrypoint
- `entrypoint.sh`: Build and launch under gdbserver
- `CMakeLists.txt`, `CMakePresets.json`: Build configuration

---

This setup is ideal for debugging C++ apps in containers with full IDE support, matching production-like environments.


## Steps to Launch

1. `docker build -t simplecppapp:debug .`
2. `docker volume create cpp_build`
3. `docker run -it --rm   --name cppdbg   --cap-add=SYS_PTRACE   --security-opt seccomp=unconfined   -p 2000:2000   -v "$PWD":/app:ro   -v "$PWD/build":/app/build:rw   -w /app   simplecppapp:debug   "abcd"`
