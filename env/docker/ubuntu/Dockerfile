FROM randomgraphics/ubuntu-vulkan-android-dev:latest
LABEL description="garnet build environment"
ARG DEBIAN_FRONTEND=noninteractive

# Install dependencies
RUN apt-get update && apt-get install -y \
    qt6-base-dev \
    && rm -rf /var/lib/apt/lists/*