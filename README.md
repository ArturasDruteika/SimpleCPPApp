# SimpleCPPApp

## Debug C++ Running in Docker with IDE Breakpoints (GDB + gdbserver)

This project demonstrates multiple correct ways to debug a C++ application running inside a Docker container, while setting breakpoints and stepping through code from an IDE on the host machine (VS Code).

The goal is to understand real-world C++ container debugging, not just make it “work once”.

## What This Project Demonstrates

- A simple C++ application (service1, service2) built with CMake
- Running the app inside Docker
- Debugging using:
  - Remote gdbserver (host GDB connects to container)
  - Pipe transport (GDB runs inside container via docker exec)
- Correct source-path mapping between container and host
- Handling STL stepping issues cleanly

## Prerequisites

- Docker + Docker Compose
- VS Code
- VS Code C/C++ extension (`ms-vscode.cpptools`)
- `gdb` installed on the host (for gdbserver mode)

## Project Layout (simplified)

```text
.
├── services/
│   ├── service1/
│   │   └── main.cpp
│   └── service2/
│       └── main.cpp
├── libs/
│   └── core/
│       ├── include_public/
│       └── src/
├── Dockerfile
├── docker-compose.yml
├── entrypoint.sh
├── CMakeLists.txt
└── .vscode/
```

All source code is mounted into the container at:

```text
/app
```

## Debugging Modes (Important)

There are two independent and valid debugging setups.

Both modes are selected via container environment variables (they do not affect your host Linux env):

- `DEBUG_WAIT=1` enables “debug mode” in `entrypoint.sh`
- `DEBUG_TRANSPORT=gdbserver|pipetransport` chooses whether the container runs `gdbserver` or sleeps (so VS Code can start GDB via pipe transport)
- `GDBSERVER_PORT` controls the port `gdbserver` listens on inside the container (default `2000`)

### Mode A — gdbserver (Recommended, Most Educational)

Host GDB → gdbserver inside container

- App runs inside container
- gdbserver runs inside container
- GDB runs on the host
- VS Code attaches remotely via TCP

This mirrors production-style remote debugging.

### Mode B — Pipe Transport

GDB runs inside container via `docker exec`

- App runs inside container
- GDB also runs inside container
- VS Code talks to GDB through Docker

This is simpler but less representative of real remote debugging.

## MODE A — gdbserver Debugging (Host GDB → Container)

### How It Works

- Container starts the app under gdbserver
- gdbserver listens on a TCP port
- VS Code launches host GDB
- Host GDB connects to gdbserver
- Breakpoints hit in container code

### Start the Container (service1 example)

```sh
docker compose up --build -d
```

Or if only a specific container would be started:

```sh
docker compose up --build -d service1
```

Check logs:

```sh
docker logs -f cpp_service1
```

You should see something like:

```text
Starting gdbserver on 0.0.0.0:2000
```

Port mapping (from `docker-compose.yml`):

- service1 → localhost:2003
- service2 → localhost:2004

### Copy the Executable to the Host (Symbols)

Host GDB needs local access to the binary with debug symbols.

```sh
mkdir -p .vscode/bin
docker cp cpp_service1:/app/bin/. .vscode/bin/
chmod +x .vscode/bin/*
```

### VS Code launch.json — gdbserver

```jsonc
{
  "version": "0.2.0",
  "configurations": [
    {
      "name": "Attach (host gdb -> gdbserver in cpp_service1)",
      "type": "cppdbg",
      "request": "launch",
      "MIMode": "gdb",
      "miDebuggerPath": "/usr/bin/gdb",
      "program": "${workspaceFolder}/.vscode/bin/service1",
      "cwd": "${workspaceFolder}",
      "miDebuggerServerAddress": "localhost:2003",
      "stopAtEntry": false,
      "externalConsole": false,
      "sourceFileMap": {
        "/app": "${workspaceFolder}"
      },
      "setupCommands": [
        { "text": "set auto-load safe-path /" },
        { "text": "-enable-pretty-printing" },
      ]
    },
    {
      "name": "Attach (host gdb -> gdbserver in cpp_service2)",
      "type": "cppdbg",
      "request": "launch",
      "MIMode": "gdb",
      "miDebuggerPath": "/usr/bin/gdb",
      "program": "${workspaceFolder}/.vscode/bin/service2",
      "cwd": "${workspaceFolder}",
      "miDebuggerServerAddress": "localhost:2004",
      "stopAtEntry": false,
      "externalConsole": false,
      "sourceFileMap": {
        "/app": "${workspaceFolder}"
      },
      "setupCommands": [
        { "text": "set auto-load safe-path /" },
        { "text": "-enable-pretty-printing" },
      ]
    }
  ]
}
```

### Notes

- Breakpoints should be placed in `.cpp` files, not headers
- Use Step Over on STL-heavy lines
- This avoids the `basic_string.h` not found issue

## MODE B — Pipe Transport Debugging (GDB inside container)

### How It Works

- Container runs normally (no gdbserver)
- VS Code launches GDB inside the container using `docker exec`
- No TCP ports involved

### Start Container Normally

```sh
DEBUG_TRANSPORT=pipetransport docker compose up --build -d
```

### VS Code launch.json — Pipe Transport

```jsonc
{
  "version": "0.2.0",
  "configurations": [
    {
      "name": "Debug via pipeTransport (gdb in container)",
      "type": "cppdbg",
      "request": "launch",
      "MIMode": "gdb",

      "program": "/app/bin/service1",
      "args": [],
      "cwd": "/app",

      "pipeTransport": {
        "pipeProgram": "docker",
        "pipeArgs": ["exec", "-i", "-t", "cpp_service1", "bash", "-lc"],
        "debuggerPath": "/usr/bin/gdb"
      },

      "externalConsole": false,

      "sourceFileMap": {
        "/app": "${workspaceFolder}"
      },

      "setupCommands": [
        { "text": "set auto-load safe-path /" },
        { "text": "-enable-pretty-printing" }
      ]
    }
  ]
}
```

### Notes

- No need to copy binaries to host
- GDB and symbols live entirely in the container
- Easier setup, but less realistic than gdbserver

## Which Mode Should I Use?

| Mode | Use When |
|---|---|
| gdbserver | Learning real remote debugging, production-like setups |
| pipeTransport | Quick debugging, minimal setup |

Both are valid. Understanding both is the real win.

## Common Pitfalls (Read This)

- ❌ Stepping into `std::string` → header not found → use `skip -gfi /usr/*` or breakpoints in your code
- ❌ Breakpoints not hit → wrong binary or missing `substitute-path`
- ❌ Symbols missing → ensure Debug build (`-g`, no stripping)

## Files of Interest

- `entrypoint.sh` — launches app under gdbserver
- `docker-compose.yml` — port mapping + capabilities
- `.vscode/launch.json` — debugger configuration
- `services/*/main.cpp` — example services

## Summary

This repository shows real, correct C++ debugging in Docker, including:

- gdbserver
- path mapping
- STL stepping pitfalls
- multiple services
- multiple debugging strategies

If you understand why both modes work, you understand Docker + C++ debugging properly.
