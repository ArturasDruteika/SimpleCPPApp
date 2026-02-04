FROM gcc:13 AS build

RUN apt-get update \
 && apt-get install -y --no-install-recommends \
    cmake ninja-build gdb gdbserver python3 libc6-dbg \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy sources and build once during image build
COPY . /app

ARG CMAKE_BUILD_TYPE=Debug
RUN cmake -S /app -B /app/build -G Ninja -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} \
 && cmake --build /app/build -j

FROM gcc:13 AS runtime

RUN apt-get update \
 && apt-get install -y --no-install-recommends \
    gdb gdbserver python3 libc6-dbg \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /app

ARG SERVICE_BIN=service1

COPY --from=build /app/build/${SERVICE_BIN} /app/bin/service
COPY --from=build /app/build/lib*.so /app/bin/
COPY entrypoint.sh /entrypoint.sh

RUN chmod +x /entrypoint.sh

ENV APP_BIN=/app/bin/service
ENV LD_LIBRARY_PATH=/app/bin
ENV DEBUG_WAIT=1
ENV DEBUG_TRANSPORT=gdbserver
ENV GDBSERVER_PORT=2000

ENTRYPOINT ["/entrypoint.sh"]
