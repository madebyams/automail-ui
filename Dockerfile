# ── Stage 1: Angular frontend ─────────────────────────────────────────────────
FROM node:22-alpine AS frontend-builder

WORKDIR /app/frontend
COPY frontend/package*.json ./
RUN npm ci
COPY frontend/ ./
RUN npm run build

# ── Stage 2: C++ backend ──────────────────────────────────────────────────────
ARG BUILD_TYPE=Release
FROM ubuntu:24.04 AS cpp-builder
ARG BUILD_TYPE

ENV DEBIAN_FRONTEND=noninteractive

# Install all system dependencies in a single layer to minimise the number of
# filesystem snapshots Kaniko has to take (each RUN triggers one snapshot).
RUN apt-get update && apt-get install -y \
        build-essential \
        ninja-build \
        python3 \
        python3-pip \
        git \
        wget \
        gpg \
        pkg-config \
        # SSL (mailio)
        libssl-dev \
        # webview dependencies on Linux
        libgtk-3-dev \
        libwebkit2gtk-4.1-dev \
    # CMake from the Kitware APT repo — no hardcoded version to maintain
    && wget -qO - https://apt.kitware.com/keys/kitware-archive-latest.asc \
        | gpg --dearmor - \
        | tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null \
    && echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ noble main' \
        | tee /etc/apt/sources.list.d/kitware.list >/dev/null \
    && apt-get update \
    && apt-get install -y cmake \
    && rm -rf /var/lib/apt/lists/*

# Conan 2
RUN pip3 install conan --break-system-packages \
    && conan profile detect --force

WORKDIR /app

# Copy source
COPY . .
# Overwrite frontend/dist with the Angular build from stage 1
COPY --from=frontend-builder /app/frontend/dist ./frontend/dist

# Register the local webview recipe into the Conan cache.
# webview/0.12.0 is not published on ConanCenter, so conan_provider.cmake
# would fail to resolve it. Running `conan create` here bakes the package
# into ~/.conan2 before CMake ever calls Conan.
RUN conan create webview-recipe/ --version 0.12.0 --test-folder ""

# Build
RUN cmake -S . -B build \
        -DCMAKE_PROJECT_TOP_LEVEL_INCLUDES=conan_provider.cmake \
        -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
        -G Ninja \
    && cmake --build build --config ${BUILD_TYPE}

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
COPY --from=cpp-builder /app/build/src/main/automailer_ui ./automail_ui

CMD ["./automail_ui"]
