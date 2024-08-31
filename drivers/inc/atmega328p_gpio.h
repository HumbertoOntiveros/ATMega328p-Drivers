#ifndef ATMEGA328P_GPIO_H
#define ATMEGA328P_GPIO_H

// Pin, port, and other GPIO-related constants definitions
#define GPIO_PIN0 0
#define GPIO_PIN1 1
#define GPIO_PIN2 2
// Add more pin definitions as needed

// Function to configure a pin as input or output
void GPIO_config(uint8_t pin, uint8_t direction);

// Function to set the state of a pin (high or low)
void GPIO_write(uint8_t pin, uint8_t value);

// Function to read the state of a pin
uint8_t GPIO_read(uint8_t pin);

// Add more GPIO-related functions and definitions as needed

#endif // ATMEGA328P_GPIO_H
