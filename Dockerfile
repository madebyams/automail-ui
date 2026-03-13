# ── Stage 1: Angular frontend ─────────────────────────────────────────────────
FROM node:22-alpine AS frontend-builder

WORKDIR /app/frontend
COPY frontend/package*.json ./
RUN npm ci
COPY frontend/ ./
RUN npm run build

# ── Stage 2: C++ backend ──────────────────────────────────────────────────────
FROM ubuntu:24.04 AS cpp-builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
        build-essential \
        ninja-build \
        python3 \
        python3-pip \
        git \
        wget \
        pkg-config \
        # SSL (mailio)
        libssl-dev \
        # webview dependencies on Linux
        libgtk-3-dev \
        libwebkit2gtk-4.1-dev \
    && rm -rf /var/lib/apt/lists/*

# CMake 4.x — adjust the version if a newer patch is available at
# https://github.com/Kitware/CMake/releases
ARG CMAKE_VERSION=4.3.0
RUN wget -qO /tmp/cmake.sh \
        "https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-linux-x86_64.sh" \
    && bash /tmp/cmake.sh --prefix=/usr/local --skip-license \
    && rm /tmp/cmake.sh

# Conan 2
RUN pip3 install conan --break-system-packages \
    && conan profile detect --force

WORKDIR /app

# Copy source
COPY . .
# Overwrite frontend/dist with the Angular build from stage 1
COPY --from=frontend-builder /app/frontend/dist ./frontend/dist

# Build
RUN cmake -S . -B build \
        -DCMAKE_PROJECT_TOP_LEVEL_INCLUDES=conan_provider.cmake \
        -DCMAKE_BUILD_TYPE=Release \
        -G Ninja \
    && cmake --build build --config Release

# ── Stage 3: Runtime image ────────────────────────────────────────────────────
FROM ubuntu:24.04 AS runtime

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
        libssl3 \
        libgtk-3-0 \
        libwebkit2gtk-4.1-0 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
# The Angular bundle is compiled into the binary as C++ byte arrays – no dist folder needed.
COPY --from=cpp-builder /app/build/src/main/main ./automail_ui

CMD ["./automail_ui"]
