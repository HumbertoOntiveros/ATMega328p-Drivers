# Compiler and flags
CC        = avr-gcc
LD        = avr-ld
OBJCOPY   = avr-objcopy
AVRDUDE   = avrdude
SIZE      = avr-size
CFLAGS    = -c -mmcu=atmega328p -std=gnu99 -Wall -O0
LDFLAGS   = -T ATMega328p.ld

# Flags for avrdude
UPLOAD_PROTOCOL  = arduino            # Change this according to your programmer
UPLOAD_PORT      = COM3               # Change this to your programming port (e.g., COM3)
UPLOAD_BAUD      = 115200             # Change this to the appropriate baud rate
TARGET_HEX       = 001led_toggle.hex  # Change this to the target you want to deploy(.hex)
TARGET_ELF       = 001led_toggle.elf  # Change this to the target you want to deploy(.elf)

# Directories
SRC_DIR = drivers/src
INC_DIR = drivers/inc
EXAMPLES_DIR = src

# Source files
OBJS = $(SRC_DIR)/atmega328p_gpio.o
# OBJS += $(SRC_DIR)/atmega328p_uart.o

# Targets
all: 001led_toggle.hex 002led_button.hex

# Compile source files in drivers/src (.s)
$(SRC_DIR)/%.o: $(SRC_DIR)/%.s $(INC_DIR)/%.h
	$(CC) $(CFLAGS) -I$(INC_DIR) -o $@ $<

# Compile source files in drivers/src (.c)
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/%.h
	$(CC) $(CFLAGS) -I$(INC_DIR) -o $@ $<

# Compile source files in src (.c)
$(EXAMPLES_DIR)/%.o: $(EXAMPLES_DIR)/%.c 
	$(CC) $(CFLAGS) -I$(INC_DIR) -o $@ $<

#  Build 001led_toggle example
001led_toggle.elf: $(EXAMPLES_DIR)/001led_toggle.o $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^
	$(OBJCOPY) 001led_toggle.elf 001led_toggle.hex -O ihex

# Build 002led_button example
002led_button.elf: $(EXAMPLES_DIR)/002led_button.o $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^
	$(OBJCOPY) 002led_button.elf 002led_buttonr.hex -O ihex

# Deploy the program to the microcontroller
deploy: $(TARGET_ELF)
	$(AVRDUDE) -p atmega328p -c $(UPLOAD_PROTOCOL) -P $(UPLOAD_PORT) -b $(UPLOAD_BAUD) -U flash:w:$(TARGET_HEX):i

# Clean object files and executables
clean:
	rm -f $(SRC_DIR)/*.o $(EXAMPLES_DIR)/*.o
	rm -f ./*.bin ./*.elf ./*.hex

