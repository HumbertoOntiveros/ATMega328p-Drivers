# ============================================================
#                SERIAL TIME SYNCHRONIZATION SCRIPT
# ============================================================
# This script establishes serial communication with a 
# microcontroller over the specified COM port. It performs 
# the following tasks:
#
# 1. Retrieves the current date and time from the system.
# 2. Adjusts the weekday so that:
#    - Sunday is represented as 1,
#    - Monday as 2,
#    - and so on until Saturday as 7.
# 3. Formats the date and time in the format:
#    "YYYY-MM-DD D HH:MM:SS", where D is the adjusted weekday.
# 4. Sends the formatted date and time to the microcontroller
#    via UART for synchronization with the system clock.
#
# This allows the microcontroller to update its internal 
# clock based on the host system's time.
# ============================================================
import serial
from datetime import datetime

def main():
    # Serial port configuration
    port = "COM4"  # Update this to the correct port
    baudrate = 9600
    timeout = 5  # Timeout for reading in seconds

    try:
        # Start serial communication
        with serial.Serial(port, baudrate, timeout=timeout) as ser:
            print(f"Connected to {port} at {baudrate} bps")

            # Send the current date and time once
            now = datetime.now()

            # Adjust day of the week so Sunday is 1, Monday is 2, ..., Saturday is 7
            day_of_week = (now.weekday() + 1) % 7 + 1  # Convert: Monday=2, Sunday=1, ..., Saturday=7

            # Format: YYYY-MM-DD D HH:MM:SS (D: day of the week as a number)
            formatted_time = now.strftime(f"%Y-%m-%d {day_of_week} %H:%M:%S")
            print(f"Sending: {formatted_time}")

            # Send the date and time to the microcontroller
            ser.write((formatted_time + "\n").encode('utf-8'))

    except serial.SerialException as e:
        print(f"Serial error: {e}")

    except KeyboardInterrupt:
        print("\nExiting...")

if __name__ == "__main__":
    main()