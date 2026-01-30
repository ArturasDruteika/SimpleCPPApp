FROM gcc:13

RUN apt-get update \
 && apt-get install -y --no-install-recommends \
    cmake ninja-build gdb gdbserver python3 libc6-dbg \
 && rm -rf /var/lib/apt/lists/*

# This sets /app as the working directory
WORKDIR /app

# Create the mountpoint for the build volume
RUN mkdir -p /app/build

COPY entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh

ENTRYPOINT ["/entrypoint.sh"]
