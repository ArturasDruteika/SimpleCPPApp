# SimpleCPPApp: Debug C++ Services in Docker with VS Code

This project shows how to debug two C++ services running in separate Docker containers using VS Code breakpoints. You can debug in two ways:

1) gdb inside the container via pipeTransport
2) host gdb attaching to gdbserver inside the container

## What’s Included

- 4 shared libs: core, math, text, time
- 2 services: service1, service2
- Each service has its own container image with only built artifacts (no source code)

## Prerequisites

- Docker
- VS Code with C++ extension

## Build + Run

```sh
docker compose up --build -d
```

Containers:
- cpp_service1 (service1)
- cpp_service2 (service2)

## Debugging Options

Choose one of the following setups. Both are supported and documented below.

### Option A: gdb inside containers (pipeTransport)

Use the provided VS Code configs in launch.json:

- Debug service1 (gdb in cpp_service1)
- Debug service2 (gdb in cpp_service2)

Both use pipeTransport and run gdb inside the container. Source mapping uses /app to your workspace.

### launch.json (pipeTransport)

```json
{
	"version": "0.2.0",
	"configurations": [
		{
			"name": "Debug service1 (gdb in cpp_service1)",
			"type": "cppdbg",
			"request": "launch",
			"MIMode": "gdb",
			"program": "/app/bin/service",
			"args": [],
			"cwd": "/app",
			"pipeTransport": {
				"pipeProgram": "docker",
				"pipeArgs": ["exec", "-i", "cpp_service1", "sh", "-lc"],
				"debuggerPath": "/usr/bin/gdb"
			},
			"externalConsole": false,
			"sourceFileMap": {
				"/app": "${workspaceFolder}"
			},
			"setupCommands": [
				{ "text": "set inferior-tty /dev/pts/1" },
				{ "text": "set auto-load safe-path /" },
				{ "text": "-enable-pretty-printing" },
				{ "text": "python import sys; sys.path.insert(0, '/usr/share/gcc/python'); import libstdcxx.v6.printers as p; p.register_libstdcxx_printers(None)"},
				{ "text": "set print pretty on" },
				{ "text": "set print object on" },
				{ "text": "set print elements 0" }
			]
		},
		{
			"name": "Debug service2 (gdb in cpp_service2)",
			"type": "cppdbg",
			"request": "launch",
			"MIMode": "gdb",
			"program": "/app/bin/service",
			"args": ["message-from-service2"],
			"cwd": "/app",
			"pipeTransport": {
				"pipeProgram": "docker",
				"pipeArgs": ["exec", "-i", "cpp_service2", "sh", "-lc"],
				"debuggerPath": "/usr/bin/gdb"
			},
			"externalConsole": false,
			"sourceFileMap": {
				"/app": "${workspaceFolder}"
			},
			"setupCommands": [
				{ "text": "set inferior-tty /dev/pts/1" },
				{ "text": "set auto-load safe-path /" },
				{ "text": "-enable-pretty-printing" },
				{ "text": "python import sys; sys.path.insert(0, '/usr/share/gcc/python'); import libstdcxx.v6.printers as p; p.register_libstdcxx_printers(None)"},
				{ "text": "set print pretty on" },
				{ "text": "set print object on" },
				{ "text": "set print elements 0" }
			]
		}
	]
}
```

### Option B: host gdb -> gdbserver inside container

This option runs gdbserver inside the container and attaches with host gdb. It is useful when you want the debugger to run on the host while the process runs in the container.

Requirements:
- gdbserver must be running in the container (see docker-compose or entrypoint)
- The container must expose a debug port (e.g., 2001)

Example VS Code config (service1):

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
			"miDebuggerServerAddress": "localhost:2001",
			"stopAtEntry": false,
			"externalConsole": false,
			"justMyCode": true,
			"sourceFileMap": {
				"/app": "${workspaceFolder}"
			},
			"setupCommands": [
				{ "text": "set auto-load safe-path /" },
				{ "text": "-enable-pretty-printing" },

				// Important: map container compile paths to host workspace paths
				{ "text": "set substitute-path /app ${workspaceFolder}" }
			]
		}
	]
}
```

### Quick debug tutorial

1. Build and start the containers:
	 ```sh
	 docker compose up --build -d
	 ```
2. Set breakpoints in:
	 - services/service1/main.cpp
	 - services/service2/main.cpp
3. In VS Code, open Run and Debug and pick:
	 - Debug service1 (gdb in cpp_service1), or
	 - Debug service2 (gdb in cpp_service2)
4. Press Start. Execution will stop on your breakpoints.

## Notes

- No source code is copied into the runtime containers.
- Each container only contains its own service binary plus required .so files.
- For gdbserver attach, ensure the binary path used in `program` matches your host copy (e.g., .vscode/bin/service1).
