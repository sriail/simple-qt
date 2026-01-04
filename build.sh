#!/bin/bash

# Simple Qt Browser Build Script
# This script automates the build process by creating the build directory
# and running CMake configuration

set -e  # Exit on error

echo "Simple Qt Browser - Build Script"
echo "================================="

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    echo "Creating build directory..."
    mkdir build
else
    echo "Build directory already exists."
fi

# Navigate to build directory
cd build

# Run CMake configuration
echo "Running CMake configuration..."
cmake ..

# Build the project
echo "Building the project..."
cmake --build .

echo ""
echo "Build complete!"
echo "Run the application with: ./build/SimpleBrowser"
