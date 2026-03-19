#!/bin/bash

# Script to properly set up ESP-IDF environment for VSCode

echo "Setting up ESP-IDF environment..."

# Check if bash is available
if ! command -v bash &> /dev/null; then
    echo "ERROR: bash is not available"
    exit 127
fi

# Set the IDF_PATH environment variable
export IDF_PATH="/home/alex/my_main_folder/Study/ESP32/ESP32_for_esp_idf/esp-idf"

# Check if IDF_PATH exists
if [ ! -d "$IDF_PATH" ]; then
    echo "ERROR: ESP-IDF directory not found at $IDF_PATH"
    exit 127
fi

# Source the ESP-IDF export script
if [ -f "$IDF_PATH/export.sh" ]; then
    source $IDF_PATH/export.sh
    echo "ESP-IDF environment sourced successfully!"
    echo "IDF_PATH is set to: $IDF_PATH"
    
    # Verify that the essential tools are available
    if command -v idf.py &> /dev/null; then
        echo "idf.py is available"
    else
        echo "ERROR: idf.py is not available in PATH"
    fi
    
    if command -v xtensa-esp32-elf-gcc &> /dev/null; then
        echo "xtensa-esp32-elf-gcc is available"
    else
        echo "WARNING: xtensa-esp32-elf-gcc is not available in PATH"
    fi
    
    echo "Environment setup complete. You can now run VSCode from this terminal."
else
    echo "ERROR: ESP-IDF export.sh not found at $IDF_PATH/export.sh"
    exit 127
fi