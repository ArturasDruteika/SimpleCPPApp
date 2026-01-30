# SimpleCPPApp: Debug C++ in Docker with IDE Breakpoints

This project demonstrates how to debug a C++ application running inside a Docker container, allowing you to hit breakpoints from your IDE (such as VS Code) on your host machine.

## How It Works

- The app is a simple C++17 program (see `main.cpp`).
- The Docker image builds the app in Debug mode and launches it under `gdbserver`.
- The container exposes port 2000 for remote debugging.
- The source code is mounted read-only into the container, while the build directory is mounted read-write (for incremental builds).
- The entrypoint script (`entrypoint.sh`) configures, builds, and launches the app under `gdbserver`.

## Prerequisites

- Docker installed on your host
- An IDE with C++ debugging support (e.g., VS Code with C++ extension)

## Quick Start


1. **Build the Docker image:**
   ```sh
   docker build -t simplecppapp:debug .
   ```

2. **Create the build volume (optional, for persistent builds):**
   ```sh
   docker volume create cpp_build
   ```

3. **Run the container:**
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
   This will start the app under `gdbserver` listening on port 2000. You can replace `"abcd"` with any arguments you want to pass to the app.

4. **Configure your IDE for remote debugging:**
   - Set up a C++ debug configuration to connect to `localhost:2000` using `gdb` or `lldb`.
   - Set the program path to `/app/build/SimpleCPPApp`.
   - Set the source path mapping from `/app` (container) to your project directory (host).

   Example for VS Code (`launch.json`):
   ```json
    {
      "version": "0.2.0",
      "configurations": [
        {
          "name": "Attach to gdbserver in Docker (bind-mount workspace)",
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

5. **Set breakpoints in your IDE and start debugging!**

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
