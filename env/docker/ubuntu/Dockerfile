FROM randomgraphics/vulkan:android-2025.2.2.7
LABEL description="garnet build environment"
ARG DEBIAN_FRONTEND=noninteractive

# Install dependencies
RUN apt-get update && apt-get install -y \
    clang-15 \
    qt6-base-dev \
    && rm -rf /var/lib/apt/lists/*