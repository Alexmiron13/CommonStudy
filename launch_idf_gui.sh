#!/bin/bash

# Script to launch ESP-IDF GUI Installation Manager bypassing the bash terminal issue

# Set the IDF_PATH environment variable
export IDF_PATH="/home/alex/my_main_folder/Study/ESP32/ESP32_for_esp_idf/esp-idf"

# Check if IDF_PATH exists
if [ ! -d "$IDF_PATH" ]; then
    echo "ERROR: ESP-IDF directory not found at $IDF_PATH"
    exit 1
fi

# Navigate to the ESP-IDF directory
cd "$IDF_PATH" || exit

# Check if the Python virtual environment is activated
if [ -z "$VIRTUAL_ENV" ]; then
    # Activate the ESP-IDF environment
    if [ -f "./export.sh" ]; then
        source ./export.sh
    else
        echo "ERROR: export.sh not found in IDF_PATH"
        exit 1
    fi
fi

# Try to run the ESP-IDF tools manager with GUI
PYTHON_PATH="$IDF_PATH/tools/idf_tools.py"

if [ -f "$PYTHON_PATH" ]; then
    echo "Attempting to launch ESP-IDF tools manager..."
    
    # Try to run the tools manager with Python
    python3 "$PYTHON_PATH" install --gui || echo "GUI option not available, showing available tools:"
    python3 "$PYTHON_PATH" list
else
    echo "ERROR: idf_tools.py not found at $PYTHON_PATH"
    exit 1
fi

# Alternative: try to run the ESP-IDF Python environment directly
if command -v idf.py &> /dev/null; then
    echo "idf.py is available, you can run: idf.py --help"
else
    echo "idf.py is not available in current PATH"
fi