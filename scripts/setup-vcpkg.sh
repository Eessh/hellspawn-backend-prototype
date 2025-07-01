#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

SCRIPT_PATH="$(realpath "$0")"
SCRIPT_DIR="$(dirname "$SCRIPT_PATH")"

# Get the project root directory (parent of the script's directory)
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
VCPKG_DIR="$PROJECT_ROOT/vcpkg"

# Check if vcpkg is already installed
if [ -d "$VCPKG_DIR" ]; then
    echo "vcpkg is already installed at $VCPKG_DIR"
    exit 0
fi

# Download the latest vcpkg source
echo "Downloading vcpkg..."
VCPKG_ZIP_URL="https://github.com/microsoft/vcpkg/archive/refs/heads/master.zip"
TEMP_ZIP="/tmp/vcpkg.zip"

curl --ssl-no-revoke -L "$VCPKG_ZIP_URL" -o "$TEMP_ZIP"

# Extract the downloaded zip file
echo "Extracting vcpkg..."
unzip -q "$TEMP_ZIP" -d "$PROJECT_ROOT"
mv "$PROJECT_ROOT/vcpkg-master" "$VCPKG_DIR"

# Clean up the temporary zip file
rm "$TEMP_ZIP"

# Run the bootstrap script
echo "Bootstrapping vcpkg..."
"$VCPKG_DIR/bootstrap-vcpkg.sh" -disableMetrics

echo "vcpkg setup completed successfully!"
