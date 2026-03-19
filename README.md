# ESP32 Project

## Setup Instructions

### Method 1: Using the setup script
Run the following command to set up the ESP-IDF environment:
```bash
./setup_environment.sh
```

Then you can run VSCode from the same terminal.

### Method 2: Using the helper script
Run the following command to source the ESP-IDF environment and launch VSCode:
```bash
./run_vscode_with_idf.sh
```

### Method 3: Manual setup
1. Source the ESP-IDF environment manually:
```bash
source /home/alex/my_main_folder/Study/ESP32/ESP32_for_esp_idf/esp-idf/export.sh
```

2. Then launch VSCode from the same terminal.

### Method 4: Install ESP-IDF Tools
If you need to install or reinstall ESP-IDF tools, run:
```bash
./install_esp_idf.sh
```

## Building the project
Once the environment is set up, you can build the project using:
```bash
idf.py build
```

## Troubleshooting
If you encounter the error "The terminal process "/usr/bin/bash" terminated with exit code: 127", try one of the following:

1. Make sure you have bash installed: `which bash`
2. Run the setup script from a terminal: `./setup_environment.sh`
3. Use the VSCode Command Palette (Ctrl+Shift+P) and run "Tasks: Run Task" to select and run one of the predefined tasks
4. If the "Open ESP-IDF Installation Manager" command still fails, use the alternative installation script: `./install_esp_idf.sh`
5. Check that the esp_idf_config.json file contains the correct paths for your system
6. The project includes an idf_terminal_init.sh script that can help initialize the ESP-IDF environment correctly
7. As an alternative, you can run the ESP-IDF Installation Manager using the Python script: `python3 run_idf_installer.py`
8. You can also use the "Run ESP-IDF Installation Manager (Python)" task from the VSCode Command Palette
9. For GUI access to the installation manager, try the "Launch ESP-IDF GUI Installation Manager" task from the VSCode Command Palette
10. You can also run the GUI installation manager directly with: `./launch_idf_gui.sh`
11. For an alternative management interface, use the "Open ESP-IDF Management Interface" task from the VSCode Command Palette
12. If all methods fail, please refer to the external setup instructions in [EXTERNAL_SETUP_INSTRUCTIONS.md](./EXTERNAL_SETUP_INSTRUCTIONS.md) for advanced troubleshooting steps
13. For comprehensive diagnostics and manual setup instructions, see [DIAGNOSTIC_AND_MANUAL_SETUP.md](./DIAGNOSTIC_AND_MANUAL_SETUP.md)