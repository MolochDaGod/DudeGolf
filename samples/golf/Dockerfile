# Multi-stage build for Super Video Golf Server
FROM ubuntu:22.04 AS builder

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libfreetype-dev \
    libsdl2-dev \
    libopenal-dev \
    libbullet-dev \
    libopus-dev \
    libunwind-dev \
    libtbb-dev \
    libsqlite3-dev \
    libcurl4-openssl-dev \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /build

# Copy source code
COPY . .

# Build crogine library
WORKDIR /build
RUN mkdir -p build && cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    make -j$(nproc) && \
    make install

# Build golf game
WORKDIR /build/samples/golf
RUN mkdir -p build && cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release \
          -DUSE_STEAM=OFF \
          .. && \
    make -j$(nproc)

# Runtime stage
FROM ubuntu:22.04

# Install runtime dependencies only
RUN apt-get update && apt-get install -y \
    libsdl2-2.0-0 \
    libopenal1 \
    libbullet3.06 \
    libsqlite3-0 \
    libcurl4 \
    libfreetype6 \
    && rm -rf /var/lib/apt/lists/*

# Create app directory
WORKDIR /app

# Copy built binary and assets
COPY --from=builder /build/samples/golf/build/golf /app/
COPY --from=builder /build/samples/golf/assets /app/assets/
COPY --from=builder /usr/local/lib/libcrogine.so* /usr/local/lib/

# Update library cache
RUN ldconfig

# Expose game server port (default 25565)
EXPOSE 25565/tcp
EXPOSE 25565/udp

# Set environment variables
ENV SERVER_MODE=1
ENV MAX_PLAYERS=4
ENV SERVER_PORT=25565

# Run the server
CMD ["./golf", "--server", "--port", "25565", "--max-players", "4"]
