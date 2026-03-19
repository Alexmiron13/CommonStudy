#!/usr/bin/env python3

"""
Script to run ESP-IDF Installation Manager without relying on bash terminal
"""

import os
import sys
import subprocess
import pathlib

def main():
    # Set the IDF_PATH environment variable
    idf_path = "/home/alex/my_main_folder/Study/ESP32/ESP32_for_esp_idf/esp-idf"
    
    # Check if IDF_PATH exists
    if not os.path.isdir(idf_path):
        print(f"ERROR: ESP-IDF directory not found at {idf_path}")
        sys.exit(1)
    
    # Set the environment
    env = os.environ.copy()
    env["IDF_PATH"] = idf_path
    
    # Add IDF tools to PATH
    tools_paths = [
        os.path.join(idf_path, "tools"),
        "/home/alex/.espressif/tools/xtensa-esp-elf-gdb/16.3_20250913/xtensa-esp-elf-gdb/bin",
        "/home/alex/.espressif/tools/riscv32-esp-elf-gdb/16.3_20250913/riscv32-esp-elf-gdb/bin",
        "/home/alex/.espressif/tools/xtensa-esp-elf/esp-15.2.0_20251204/xtensa-esp-elf/bin",
        "/home/alex/.espressif/tools/riscv32-esp-elf/esp-15.2.0_20251204/riscv32-esp-elf/bin",
        "/home/alex/.espressif/tools/esp32ulp-elf/2.38_20240113/esp32ulp-elf/bin",
        "/home/alex/.espressif/tools/openocd-esp32/v0.12.0-esp32-20251215/openocd-esp32/bin",
        "/home/alex/.espressif/python_env/idf6.1_py3.10_env/bin"
    ]
    
    # Build the PATH
    current_path = env.get("PATH", "")
    new_path = ":".join(tools_paths + [current_path])
    env["PATH"] = new_path
    
    # Try to run the ESP-IDF installer GUI
    installer_script = os.path.join(idf_path, "tools", "install.py")
    
    if os.path.exists(installer_script):
        print("Running ESP-IDF Installation Manager...")
        try:
            result = subprocess.run([
                sys.executable, installer_script, "--interactive"
            ], env=env, check=True)
            print("Installation manager completed successfully")
        except subprocess.CalledProcessError as e:
            print(f"Error running installation manager: {e}")
            sys.exit(e.returncode)
        except FileNotFoundError:
            print("Installation manager script not found, trying alternative approach...")
            
            # Alternative: run the idf_tools.py script
            tools_script = os.path.join(idf_path, "tools", "idf_tools.py")
            if os.path.exists(tools_script):
                try:
                    result = subprocess.run([
                        sys.executable, tools_script, "--help"
                    ], env=env, check=True)
                    print("ESP-IDF tools are accessible")
                except subprocess.CalledProcessError as e:
                    print(f"Error accessing ESP-IDF tools: {e}")
                    sys.exit(e.returncode)
    else:
        print(f"Installer script not found at {installer_script}")
        
        # Try to run idf_tools.py directly with correct arguments
        tools_script = os.path.join(idf_path, "tools", "idf_tools.py")
        if os.path.exists(tools_script):
            try:
                print("Running ESP-IDF tools manager...")
                # Show available tools first
                result = subprocess.run([
                    sys.executable, tools_script, "list"
                ], env=env, check=True)
                
                print("\nTo install all tools, run:")
                print(f"python {tools_script} install")
                print("Or to see more options:")
                print(f"python {tools_script} --help")
                
            except subprocess.CalledProcessError as e:
                print(f"Error running ESP-IDF tools: {e}")
                sys.exit(e.returncode)
        else:
            print(f"Neither installer nor tools script found")
            sys.exit(1)

if __name__ == "__main__":
    main()