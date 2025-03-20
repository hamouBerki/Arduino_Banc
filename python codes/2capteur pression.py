# -*- coding: utf-8 -*-
"""
Created on Thu Mar 20 14:34:27 2025

@author: spinalcom
"""


import serial

# Set the correct port (Check Device Manager on Windows or `ls /dev/tty*` on Linux/macOS)
SERIAL_PORT = "COM6"  # Change to "COMx" on Windows (e.g., "COM3")
BAUD_RATE = 115200

try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    print(f"Connected to {SERIAL_PORT}")
   
    while True:
        line = ser.readline().decode("utf-8").strip()
        if line:
            try:
                sensor_id, pressure, temperature = line.split(",")
                print(f"Sensor {sensor_id} - Pressure: {pressure} Pa | Temperature: {temperature} °C")
            except ValueError:
                print(f"Invalid data: {line}")  # Handle errors if data is malformed

except KeyboardInterrupt:
    print("Exiting...")
    ser.close()
except Exception as e:
    print("Error:", e)
