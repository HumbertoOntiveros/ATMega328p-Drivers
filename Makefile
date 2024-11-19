# Compiler and flags
CC               = avr-gcc
LD               = avr-ld
OBJCOPY          = avr-objcopy
OBJDUMP          = avr-objdump
AVRDUDE          = avrdude
SIZE             = avr-size
CFLAGS           = -mmcu=atmega328p -std=gnu99 -Wall -O0 -fno-lto -g3
LDFLAGS          = -mmcu=atmega328p

# Flags for avrdude
UPLOAD_PROTOCOL  ?= arduino                # Change this according to your programmer
UPLOAD_PORT      ?= COM8                   # Change this to your programming port (e.g., COM3)
UPLOAD_BAUD      ?= 115200                 # Change this to the appropriate baud rate
TARGET_LSS       ?= 008spi_message_rcv_it.lss # Target in .lss format
TARGET_HEX       ?= 008spi_message_rcv_it.hex # Target in .hex format
TARGET_ELF       ?= 008spi_message_rcv_it.elf # Target in .elf format

# Directories
SRC_DIR          = drivers/src
INC_DIR          = drivers/inc
BSP_DIR          = bsp
EXAMPLES_DIR     = src
MONITOR_PATH     := ./scripts/serial_monitor.ps1

# Source files
OBJS =  $(SRC_DIR)/syscalls.o
OBJS += $(SRC_DIR)/atmega328p_i2c.o
OBJS += $(SRC_DIR)/atmega328p_spi.o
OBJS += $(SRC_DIR)/atmega328p_gpio.o

# Targets
all:	000pilot_example.elf \
        008spi_message_rcv_it.elf \
        007spi_cmd_handling.elf \
        006spi_txonly_arduino.elf \
        005spi_tx.elf \
    	004led_button_toggle_int.elf \
		003hello_world.elf \
		002led_button_toggle.elf \
		001led_toggle.elf 
	@echo "Build complete for the following examples:"
	@echo " - 008spi_message_rcv_it"
	@echo " - 007spi_cmd_handling"
	@echo " - 006spi_txonly_arduino"
	@echo " - 005spi_tx"
	@echo " - 004led_button_toggle_int"
	@echo " - 003hello_world"
	@echo " - 002led_button_toggle"
	@echo " - 001led_toggle"
	@echo " - 000pilot_example"

# Compile source files in src (.c)
$(EXAMPLES_DIR)/%.o: $(EXAMPLES_DIR)/%.c 
	@echo "Compiling example source: $<"
	$(CC) $(CFLAGS) -c -I$(INC_DIR) -I$(BSP_DIR) -o $@ $<

# Compile source files in bsp (.c)
$(BSP_DIR)/%.o: $(BSP_DIR)/%.c $(BSP_DIR)/%.h
	@echo "Compiling bsp source: $<"
	$(CC) $(CFLAGS) -c -I$(BSP_DIR) -I$(INC_DIR) -o $@ $<

# Compile source files in drivers/src (.c)
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/%.h
	@echo "Compiling driver source: $<"
	$(CC) $(CFLAGS) -c -I$(INC_DIR) -o $@ $<

#  Build 008spi_message_rcv_it example
008spi_message_rcv_it.elf: $(EXAMPLES_DIR)/008spi_message_rcv_it.o $(OBJS)
	@echo "Linking 008spi_message_rcv_it.elf..."
	$(CC) $(LDFLAGS) -o $@ $^
	@echo "Creating HEX file for 008spi_message_rcv_it..."
	$(OBJCOPY) 008spi_message_rcv_it.elf 008spi_message_rcv_it.hex -O ihex
	@echo "Build complete: 008spi_message_rcv_it.elf"

#  Build 007spi_cmd_handling example
007spi_cmd_handling.elf: $(EXAMPLES_DIR)/007spi_cmd_handling.o $(OBJS)
	@echo "Linking 007spi_cmd_handling.elf..."
	$(CC) $(LDFLAGS) -o $@ $^
	@echo "Creating HEX file for 007spi_cmd_handling..."
	$(OBJCOPY) 007spi_cmd_handling.elf 007spi_cmd_handling.hex -O ihex
	@echo "Build complete: 007spi_cmd_handling.elf"

#  Build 006spi_txonly_arduino example
006spi_txonly_arduino.elf: $(EXAMPLES_DIR)/006spi_txonly_arduino.o $(OBJS)
	@echo "Linking 006spi_txonly_arduino.elf..."
	$(CC) $(LDFLAGS) -o $@ $^
	@echo "Creating HEX file for 006spi_txonly_arduino..."
	$(OBJCOPY) 006spi_txonly_arduino.elf 006spi_txonly_arduino.hex -O ihex
	@echo "Build complete: 006spi_txonly_arduino.elf"

#  Build 005spi_tx example
005spi_tx.elf: $(EXAMPLES_DIR)/005spi_tx.o $(OBJS)
	@echo "Linking 005spi_tx.elf..."
	$(CC) $(LDFLAGS) -o $@ $^
	@echo "Creating HEX file for 005spi_tx..."
	$(OBJCOPY) 005spi_tx.elf 005spi_tx.hex -O ihex
	@echo "Build complete: 005spi_tx.elf"

#  Build 004led_button_toggle_int example
004led_button_toggle_int.elf: $(EXAMPLES_DIR)/004led_button_toggle_int.o $(OBJS)
	@echo "Linking 004led_button_toggle_int.elf..."
	$(CC) $(LDFLAGS) -o $@ $^
	@echo "Creating HEX file for 004led_button_toggle_int..."
	$(OBJCOPY) 004led_button_toggle_int.elf 004led_button_toggle_int.hex -O ihex
	@echo "Build complete: 004led_button_toggle_int.elf"

#  Build 003hello_world example
003hello_world.elf: $(EXAMPLES_DIR)/003hello_world.o $(OBJS)
	@echo "Linking 003hello_world.elf..."
	$(CC) $(LDFLAGS) -o $@ $^
	@echo "Creating HEX file for 003hello_world..."
	$(OBJCOPY) 003hello_world.elf 003hello_world.hex -O ihex
	@echo "Build complete: 003hello_world.elf"

#  Build 002led_button example
002led_button_toggle.elf: $(EXAMPLES_DIR)/002led_button_toggle.o $(OBJS)
	@echo "Linking 002led_button_toggle.elf..."
	$(CC) $(LDFLAGS) -o $@ $^
	@echo "Creating HEX file for 002led_button_toggle..."
	$(OBJCOPY) 002led_button_toggle.elf 002led_button_toggle.hex -O ihex
	@echo "Build complete: 002led_button_toggle.elf"

#  Build 001led_toggle example
001led_toggle.elf: $(EXAMPLES_DIR)/001led_toggle.o $(OBJS)
	@echo "Linking 001led_toggle.elf..."
	$(CC) $(LDFLAGS) -o $@ $^
	@echo "Creating HEX file for 001led_toggle..."
	$(OBJCOPY) 001led_toggle.elf 001led_toggle.hex -O ihex
	@echo "Build complete: 001led_toggle.elf"

#  Build 000pilot_example example
000pilot_example.elf: $(EXAMPLES_DIR)/000pilot_example.o
	@echo "Linking 000pilot_example.elf..."
	$(CC) $(LDFLAGS) -o $@ $^
	@echo "Creating HEX file for 000pilot_example..."
	$(OBJCOPY) 000pilot_example.elf 000pilot_example.hex -O ihex
	@echo "Build complete: 000pilot_example.elf"

# Deploy the program to the microcontroller
deploy: clean $(TARGET_ELF) $(TARGET_LSS)
	@echo "Deploying $(TARGET_HEX) to microcontroller..."
	$(AVRDUDE) -p atmega328p -c $(UPLOAD_PROTOCOL) -P $(UPLOAD_PORT) -b $(UPLOAD_BAUD) -U flash:w:$(TARGET_HEX):i
	@echo "Deployment process finished."

# Generate a .lss file
$(TARGET_LSS): $(TARGET_ELF)
	@echo "Generating LSS file from $(TARGET_ELF)..."
	$(OBJDUMP) -h -S $(TARGET_ELF) > $(TARGET_LSS)
	@echo "LSS file generated: $(TARGET_LSS)"

# Runs the serial monitor script using PowerShell
serial-monitor:
	@powershell -ExecutionPolicy Bypass -File $(MONITOR_PATH)

# Clean object files and executables
clean:
	@echo "Cleaning up build artifacts..."
	rm -f $(SRC_DIR)/*.o $(EXAMPLES_DIR)/*.o $(BSP_DIR)/*.o
	rm -f ./*.bin ./*.elf ./*.hex ./*.lss
	@echo "Clean complete."
