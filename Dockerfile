# Use Ubuntu as base image for better compatibility
FROM ubuntu:22.04

# Prevent interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install build dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libcurl4-openssl-dev \
    nlohmann-json3-dev \
    git \
    devscripts \
    debhelper \
    dh-make \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy source files
COPY . .

# Clean up existing CMake files and build artifacts
RUN rm -rf CMakeFiles CMakeCache.txt cmake_install.cmake Makefile build/

# Ensure proper permissions
RUN chmod +x debian/rules

# Build the Debian package
RUN dpkg-buildpackage -us -uc -b

# List contents of parent directory to verify .deb creation
RUN ls -la ../

# The built .deb package will be in the parent directory
