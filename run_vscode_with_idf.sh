#!/bin/bash

# Script to source ESP-IDF environment and launch VSCode

echo "Sourcing ESP-IDF environment and launching VSCode..."

# Set the IDF_PATH environment variable
export IDF_PATH="/home/alex/my_main_folder/Study/ESP32/ESP32_for_esp_idf/esp-idf"

# Check if IDF_PATH exists
if [ ! -d "$IDF_PATH" ]; then
    echo "ERROR: ESP-IDF directory not found at $IDF_PATH"
    exit 1
fi

# Source the ESP-IDF export script
if [ -f "$IDF_PATH/export.sh" ]; then
    source $IDF_PATH/export.sh
    echo "ESP-IDF environment sourced successfully!"
    echo "IDF_PATH is set to: $IDF_PATH"
    
    # Launch VSCode with the sourced environment
    echo "Launching VSCode with ESP-IDF environment..."
    exec code .
else
    echo "ERROR: ESP-IDF export.sh not found at $IDF_PATH/export.sh"
    exit 1
fi