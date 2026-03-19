#!/bin/bash

# Script to initialize ESP-IDF environment specifically for the extension
# This script addresses the issue where the ESP-IDF extension cannot find bash

# Ensure we're using the correct bash
if [ -z "$BASH_VERSION" ]; then
    exec /usr/bin/bash "$0" "$@"
fi

# Set the IDF_PATH environment variable
export IDF_PATH="/home/alex/my_main_folder/Study/ESP32/ESP32_for_esp_idf/esp-idf"

# Check if IDF_PATH exists
if [ ! -d "$IDF_PATH" ]; then
    echo "ERROR: ESP-IDF directory not found at $IDF_PATH"
    exit 127
fi

# Check if export.sh exists
if [ ! -f "$IDF_PATH/export.sh" ]; then
    echo "ERROR: ESP-IDF export.sh not found at $IDF_PATH/export.sh"
    exit 127
fi

# Source the ESP-IDF export script
source "$IDF_PATH/export.sh"

# Verify that essential tools are available
if ! command -v idf.py &> /dev/null; then
    echo "ERROR: idf.py is not available in PATH"
    exit 127
fi

if ! command -v python &> /dev/null && ! command -v python3 &> /dev/null; then
    echo "ERROR: Python is not available in PATH"
    exit 127
fi

echo "ESP-IDF environment initialized successfully"
echo "IDF_PATH is set to: $IDF_PATH"
echo "ESP-IDF version: $ESP_IDF_VERSION"

# If arguments were passed, execute them
if [ $# -gt 0 ]; then
    exec "$@"
else
    # Otherwise, just keep the environment ready
    exec bash
fi