FROM gcc:13

RUN apt-get update \
 && apt-get install -y --no-install-recommends \
    cmake ninja-build gdb gdbserver python3 libc6-dbg \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /app
RUN mkdir -p /app/build

# Copy sources first (so we can build them during docker build)
COPY . /app

# Build args so you can switch Debug/Release at build time
ARG CMAKE_BUILD_TYPE=Debug

# Configure + build at image build time
RUN cmake -S /app -B /app/build -G Ninja -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} \
 && cmake --build /app/build -j

# Keep your entrypoint (runtime behavior)
COPY entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh

ENTRYPOINT ["/entrypoint.sh"]
