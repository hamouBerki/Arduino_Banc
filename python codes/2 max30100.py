# -*- coding: utf-8 -*-
"""
Ce code permet  de lire les données des max30100 connecter au MUX

Rq : Assuez vous de bien televerser le bon code sur arduino ensuite, fermer la fenetre arduino et
ouvrez un idel python et lancé ce code.
Sur la console vous pourez lire le resultat du code téléverser précédement.
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
            print(line)  # Print the received data
except KeyboardInterrupt:
    print("Exiting...")
    ser.close()
except Exception as e:
    print("Error:", e)