# SimpleCPPApp: Debug C++ Services in Docker with VS Code

This project shows how to debug two C++ services running in separate Docker containers using VS Code breakpoints. The build happens inside the containers and gdb runs inside each container (no gdbserver).

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

## Debugging (gdb inside containers)

Use the provided VS Code configs in launch.json:

- Debug service1 (gdb in cpp_service1)
- Debug service2 (gdb in cpp_service2)

Both use pipeTransport and run gdb inside the container. Source mapping uses /app to your workspace.

### launch.json (current)

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
