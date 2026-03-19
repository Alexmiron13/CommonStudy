#!/bin/bash

# Script to install ESP-IDF tools

echo "Installing ESP-IDF tools..."

# Set the IDF_PATH environment variable
export IDF_PATH="/home/alex/my_main_folder/Study/ESP32/ESP32_for_esp_idf/esp-idf"

# Check if IDF_PATH exists
if [ ! -d "$IDF_PATH" ]; then
    echo "ERROR: ESP-IDF directory not found at $IDF_PATH"
    exit 1
fi

# Navigate to the ESP-IDF directory
cd "$IDF_PATH" || exit

# Run the installation script
if [ -f "./install.sh" ]; then
    echo "Running ESP-IDF installation script..."
    ./install.sh
else
    echo "ERROR: ESP-IDF install.sh not found at $IDF_PATH/install.sh"
    exit 1
fi