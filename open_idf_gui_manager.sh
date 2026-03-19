#!/bin/bash

# Script to open ESP-IDF GUI Manager or alternative interface

echo "Attempting to open ESP-IDF management interface..."

# Set the IDF_PATH environment variable
export IDF_PATH="/home/alex/my_main_folder/Study/ESP32/ESP32_for_esp_idf/esp-idf"

# Check if IDF_PATH exists
if [ ! -d "$IDF_PATH" ]; then
    echo "ERROR: ESP-IDF directory not found at $IDF_PATH"
    exit 1
fi

# Navigate to the ESP-IDF directory
cd "$IDF_PATH" || exit

# Source the ESP-IDF environment
if [ -f "./export.sh" ]; then
    source ./export.sh
    echo "ESP-IDF environment sourced"
else
    echo "ERROR: export.sh not found in IDF_PATH"
    exit 1
fi

# Check if idf.py is available
if command -v idf.py &> /dev/null; then
    echo "idf.py is available"
    echo ""
    echo "Available ESP-IDF commands:"
    echo "  idf.py --help          : Show available commands"
    echo "  idf.py menuconfig      : Open configuration menu"
    echo "  idf.py build           : Build the project"
    echo "  idf.py flash           : Flash the project"
    echo "  idf.py monitor         : Monitor serial output"
    echo ""
    echo "Since the GUI Installation Manager is not available due to terminal issues,"
    echo "you can use these command-line alternatives:"
    echo ""
    echo "For project configuration: idf.py menuconfig"
    echo "For checking tools: idf.py list"
    echo "For installing tools: idf.py install"
    echo ""
    echo "Would you like to run 'idf.py --help' now? (y/n)"
    read -r response
    if [[ $response =~ ^[Yy]$ ]]; then
        idf.py --help
    fi
else
    echo "ERROR: idf.py is not available in PATH"
    exit 1
fi