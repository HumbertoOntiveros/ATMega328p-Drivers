# Compiler and flags
CC               = avr-gcc
LD               = avr-ld
OBJCOPY          = avr-objcopy
OBJDUMP          = avr-objdump
AVRDUDE          = avrdude
SIZE             = avr-size
CFLAGS           = -mmcu=atmega328p -std=gnu99 -Wall -Os
LDFLAGS          = -mmcu=atmega328p

# Flags for avrdude
UPLOAD_PROTOCOL  = arduino                # Change this according to your programmer
UPLOAD_PORT      = COM4                   # Change this to your programming port (e.g., COM3)
UPLOAD_BAUD      = 115200                 # Change this to the appropriate baud rate
TARGET_LSS       = 000pilot_example.lss      # Target in .lss format
TARGET_HEX       = 000pilot_example.hex      # Target in .hex format
TARGET_ELF       = 000pilot_example.elf      # Target in .elf format

# Directories
SRC_DIR          = drivers/src
INC_DIR          = drivers/inc
EXAMPLES_DIR     = src

# Source files
OBJS = $(SRC_DIR)/atmega328p_gpio.o
# OBJS += $(SRC_DIR)/atmega328p_uart.o

# Targets
all: 000pilot_example.elf 001led_toggle.elf
	@echo "Build complete for the following examples:"
	@echo " - 000pilot_example"
	@echo " - 001led_toggle"
#	@echo " - 002led_button"

###################################################
# Compile source files in drivers/src (.c)
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/%.h
	@echo "Compiling driver source: $<"
	$(CC) $(CFLAGS) -c -I$(INC_DIR) -o $@ $<

# Compile source files in src (.c)
$(EXAMPLES_DIR)/%.o: $(EXAMPLES_DIR)/%.c 
	@echo "Compiling example source: $<"
	$(CC) $(CFLAGS) -c -I$(INC_DIR) -o $@ $<

# Compile source files in drivers/src (.c)
#$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
#	$(CC) $(CFLAGS) -c $< -o $@

# Compile source files in src (.c)
#$(EXAMPLES_DIR)/%.o: $(EXAMPLES_DIR)/%.c
#	$(CC) $(CFLAGS) -c $< -o $@

####################################################

#  Build 000pilot_example example
000pilot_example.elf: $(EXAMPLES_DIR)/000pilot_example.o
	@echo "Linking 000pilot_example.elf..."
	$(CC) $(LDFLAGS) -o $@ $^
	@echo "Creating HEX file for 000pilot_example..."
	$(OBJCOPY) 000pilot_example.elf 000pilot_example.hex -O ihex
	@echo "Build complete: 000pilot_example.elf"

#  Build 001led_toggle example
001led_toggle.elf: $(EXAMPLES_DIR)/001led_toggle.o $(OBJS)
	@echo "Linking 001led_toggle.elf..."
	$(CC) $(LDFLAGS) -o $@ $^
	@echo "Creating HEX file for 001led_toggle..."
	$(OBJCOPY) 001led_toggle.elf 001led_toggle.hex -O ihex
	@echo "Build complete: 001led_toggle.elf"

# Build 002led_button example
002led_button.elf: $(EXAMPLES_DIR)/002led_button.o $(OBJS)
	@echo "Linking 002led_button.elf..."
	$(CC) $(LDFLAGS) -o $@ $^
	@echo "Creating HEX file for 002led_button..."
	$(OBJCOPY) 002led_button.elf 002led_buttonr.hex -O ihex
	@echo "Build complete: 002led_button.elf"

# Deploy the program to the microcontroller
deploy: $(TARGET_ELF) $(TARGET_LSS)
	@echo "Deploying $(TARGET_HEX) to microcontroller..."
	$(AVRDUDE) -p atmega328p -c $(UPLOAD_PROTOCOL) -P $(UPLOAD_PORT) -b $(UPLOAD_BAUD) -U flash:w:$(TARGET_HEX):i
	@echo "Deployment process finished."

# Generate a .lss file
$(TARGET_LSS): $(TARGET_ELF)
	@echo "Generating LSS file from $(TARGET_ELF)..."
	$(OBJDUMP) -h -S $(TARGET_ELF) > $(TARGET_LSS)
	@echo "LSS file generated: $(TARGET_LSS)"

# Clean object files and executables
clean:
	@echo "Cleaning up build artifacts..."
	rm -f $(SRC_DIR)/*.o $(EXAMPLES_DIR)/*.o
	rm -f ./*.bin ./*.elf ./*.hex ./*.lss
	@echo "Clean complete."
