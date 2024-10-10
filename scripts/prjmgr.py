import os
import json
import argparse
from datetime import datetime

target_line = f"TARGET_LSS       ?= 003hello_world.lss # Target in .lss format\n"
target_hex_line = f"TARGET_HEX       ?= 003hello_world.hex # Target in .hex format\n"
target_elf_line = f"TARGET_ELF       ?= 003hello_world.elf # Target in .elf format\n"

# Cargar las plantillas desde el archivo JSON
with open('scripts/templates.json', 'r') as file:
    templates = json.load(file)

# Función para reemplazar placeholders en la plantilla
def replace_placeholders(template, driver_name):
    current_time = datetime.now()
    driver_name_upper = driver_name.upper()
    return template.replace("{{driver_name}}", driver_name) \
        .replace("{{driver_name_upper}}", driver_name_upper) \
        .replace("{{date}}", current_time.strftime("%d/%m/%Y %H:%M:%S"))

# Función para actualizar el Makefile en caso de ejemplo
def update_makefile_example(action, driver_name):
    with open('Makefile', 'r') as file:
        lines = file.readlines()

    # Actualizar las variables TARGET_LSS, TARGET_HEX, TARGET_ELF y el objetivo 'all'
    target_line = f"TARGET_LSS       ?= {driver_name}.lss # Target in .lss format\n"
    target_hex_line = f"TARGET_HEX       ?= {driver_name}.hex # Target in .hex format\n"
    target_elf_line = f"TARGET_ELF       ?= {driver_name}.elf # Target in .elf format\n"
    elf_target = f"{driver_name}.elf"
    
    if action == 'add':
        # Agregar ejemplo a las variables TARGET_LSS, TARGET_HEX, TARGET_ELF y 'all'
        for i, line in enumerate(lines):
            if line.startswith("TARGET_LSS"):
                lines[i] = target_line
            elif line.startswith("TARGET_HEX"):
                lines[i] = target_hex_line
            elif line.startswith("TARGET_ELF"):
                lines[i] = target_elf_line
            elif line.startswith("all:"):
                if elf_target not in line:
                    lines.insert(i + 1, "        " + elf_target + " \\\n")
            elif line.startswith("	@echo \"Build complete for the following examples:\""):
                    lines.insert(i + 1, "	@echo \" - " + driver_name + "\"\n")
            elif line.startswith("#  Build "):
                build_target = f"\n#  Build {driver_name} example\n"
                elf_build = f"{driver_name}.elf: $(EXAMPLES_DIR)/{driver_name}.o $(OBJS)\n"
                elf_build += f"\t@echo \"Linking {driver_name}.elf...\"\n"
                elf_build += f"\t$(CC) $(LDFLAGS) -o $@ $^\n"
                elf_build += f"\t@echo \"Creating HEX file for {driver_name}...\"\n"
                elf_build += f"\t$(OBJCOPY) {driver_name}.elf {driver_name}.hex -O ihex\n"
                elf_build += f"\t@echo \"Build complete: {driver_name}.elf\"\n"
                lines.insert(i - 1, build_target + elf_build)
                break

    elif action == 'remove':
        # Eliminar las referencias al ejemplo
        for i, line in enumerate(lines):
            if target_line in line:
                lines[i] = line.replace(f"{driver_name}", "003hello_world")
            elif target_hex_line in line:
                lines[i] = line.replace(f"{driver_name}", "003hello_world")
            elif target_elf_line in line:
                lines[i] = line.replace(f"{driver_name}", "003hello_world")
            elif elf_target in line:
                lines[i] = line.replace("       " + elf_target + " \\\n", "")
            elif "	@echo \" - " + driver_name + "\"" in line:
                del lines[i] #= line.replace("	@echo \" - " + driver_name + "\"", "")
            elif line.startswith(f"#  Build {driver_name} example"):
                del lines[i - 1:i + 7]  # Elimina el bloque de construcción del ejemplo

    with open('Makefile', 'w') as file:
        file.writelines(lines)

# Función para actualizar el Makefile en caso de driver o BSP
def update_makefile_driver(action, driver_name):
    with open('Makefile', 'r') as file:
        lines = file.readlines()

    # Actualizar la variable OBJS con nuevo driver o BSP 
    objs_line = f"OBJS += $(SRC_DIR)/{driver_name}.o\n"
    
    if action == 'add':
        # Agregar driver a la variable OBJS
        for i, line in enumerate(lines):
            if line.startswith("OBJS"):
                lines.insert(i + 1, objs_line)
                break
    elif action == 'remove':
        # Remover driver de la variable OBJS
        lines = [line for line in lines if line.strip() != objs_line.strip()]
    with open('Makefile', 'w') as file:
        file.writelines(lines)

# Function to create a new driver based on templates
def create_driver(driver_name):
    # File paths for the header and source files
    header_file_path = f'drivers/inc/{driver_name}.h'
    source_file_path = f'drivers/src/{driver_name}.c'

    # Check if both files already exist
    if os.path.exists(header_file_path) and os.path.exists(source_file_path):
        print(f'Driver {driver_name} already exists')
    else:
        # Create header file (.h) if it doesn't exist
        if not os.path.exists(header_file_path):
            header_content = replace_placeholders(templates['driver_template']['header'], driver_name)
            with open(header_file_path, 'w') as header_file:
                header_file.write(header_content)
            print(f'Created header file: {header_file_path}')
        
        # Create source file (.c) if it doesn't exist
        if not os.path.exists(source_file_path):
            source_content = replace_placeholders(templates['driver_template']['source'], driver_name)
            with open(source_file_path, 'w') as source_file:
                source_file.write(source_content)
            print(f'Created source file: {source_file_path}')

        # Update Makefile
        update_makefile_driver('add', driver_name)

# Function to remove a driver
def remove_driver(driver_name):
    header_file_path = f'drivers/inc/{driver_name}.h'
    source_file_path = f'drivers/src/{driver_name}.c'

    if os.path.exists(header_file_path):
        os.remove(header_file_path)
        print(f'Removed header file: {header_file_path}')
    if os.path.exists(source_file_path):
        os.remove(source_file_path)
        print(f'Removed source file: {source_file_path}')
    
    # Update Makefile
    update_makefile_driver('remove', driver_name)
    
    if not os.path.exists(header_file_path) and not os.path.exists(source_file_path):
        print(f'Driver {driver_name} has been removed successfully! All files cleared.')

# Function to create an example using the driver
def create_example(driver_name):
    # Path for the example's C source file
    example_file_path = f'src/{driver_name}.c'

    # Check if the example file already exists
    if os.path.exists(example_file_path):
        print(f'Example {driver_name} already exists')
    else:
        # Create the example C file if it doesn't exist
        main_content = replace_placeholders(templates['example_template']['main'], driver_name)
        with open(example_file_path, 'w') as main_file:
            main_file.write(main_content)
        
        # Update Makefile
        update_makefile_example('add', driver_name)

        print(f'Created example: {example_file_path}')

# Function to remove an example
def remove_example(driver_name):
    example_file_path = f'src/{driver_name}.c'

    if os.path.exists(example_file_path):
        os.remove(example_file_path)
        # Update Makefile
        update_makefile_example('remove', driver_name)
        print(f'Removed example file: {example_file_path}')
    else:
        print(f'Example {driver_name} does not exist')

# Function to create a BSP file
def create_bsp(bsp_name):
    # Paths for BSP header and source files
    header_file_path = f'bsp/{bsp_name}.h'
    source_file_path = f'bsp/{bsp_name}.c'

    # Check if both BSP files already exist
    if os.path.exists(header_file_path) and os.path.exists(source_file_path):
        print(f'BSP {bsp_name} already exists')
    else:
        # Create the BSP header file if it doesn't exist
        if not os.path.exists(header_file_path):
            header_content = replace_placeholders(templates['bsp_template']['header'], bsp_name)
            with open(header_file_path, 'w') as header_file:
                header_file.write(header_content)
            print(f'Created BSP header: {header_file_path}')
        
        # Create the BSP source file if it doesn't exist
        if not os.path.exists(source_file_path):
            source_content = replace_placeholders(templates['bsp_template']['source'], bsp_name)
            with open(source_file_path, 'w') as source_file:
                source_file.write(source_content)
            print(f'Created BSP source: {source_file_path}')

        # Update Makefile
        update_makefile_driver('add', bsp_name)

# Function to remove a BSP
def remove_bsp(bsp_name):
    header_file_path = f'bsp/{bsp_name}.h'
    source_file_path = f'bsp/{bsp_name}.c'

    if os.path.exists(header_file_path):
        os.remove(header_file_path)
        print(f'Removed BSP header file: {header_file_path}')
    if os.path.exists(source_file_path):
        os.remove(source_file_path)
        print(f'Removed BSP source file: {source_file_path}')
    
    # Update Makefile
    update_makefile_driver('remove', bsp_name)

    if not os.path.exists(header_file_path) and not os.path.exists(source_file_path):
        print(f'BSP {bsp_name} has been removed successfully! All files cleared.')

# Configuración de argparse para manejar argumentos de línea de comandos
def main():
    parser = argparse.ArgumentParser(description='Project automation script for managing drivers, examples, and BSPs.')
    parser.add_argument('--create-driver', type=str, help='Create a new driver with the given name')
    parser.add_argument('--remove-driver', type=str, help='Remove an existing driver by name')
    parser.add_argument('--create-example', type=str, help='Create a new example for the given driver')
    parser.add_argument('--remove-example', type=str, help='Remove an existing example by name')
    parser.add_argument('--create-bsp', type=str, help='Create a new BSP with the given name')
    parser.add_argument('--remove-bsp', type=str, help='Remove an existing BSP by name')

    args = parser.parse_args()

    if args.create_driver:
        create_driver(args.create_driver)

    if args.remove_driver:
        remove_driver(args.remove_driver)

    if args.create_example:
        create_example(args.create_example)

    if args.remove_example:
        remove_example(args.remove_example)

    if args.create_bsp:
        create_bsp(args.create_bsp)

    if args.remove_bsp:
        remove_bsp(args.remove_bsp)

if __name__ == '__main__':
    main()
