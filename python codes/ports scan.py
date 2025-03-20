
import serial
import time


#ce code est utile pour lister les COM disponibles sur arduino

# Change if necessary
PORT = "COM6"  
BAUDRATE = 115200  # Must match Arduino's Serial.begin(115200)

try:
    # Open Serial Port
    arduino = serial.Serial(PORT, BAUDRATE, timeout=1)
    time.sleep(2)  # Give Arduino time to initialize

    print(f"Connected to {PORT}, reading sensor data...\n")

    while True:
        if arduino.in_waiting > 0:  # Check if data is available
            try:
                data = arduino.readline().decode('utf-8').strip()  # Read and decode
                if data:  # Ensure it's not empty
                    print("Received:", data)

                    # If Arduino sends "Heart rate: 75bpm / SpO2: 98%"
                    if "Heart rate:" in data and "SpO2:" in data:
                        parts = data.replace("Heart rate:", "").replace("bpm / SpO2:", "").split()
                        heart_rate = parts[0]
                        spo2 = parts[1] if len(parts) > 1 else "N/A"
                        print(f"Heart Rate: {heart_rate} bpm, SpO2: {spo2}%")

                    # If Arduino sends "75,98" (Formatted as CSV)
                    elif "," in data:
                        values = data.split(",")
                        if len(values) == 2:
                            heart_rate, spo2 = values
                            print(f"Heart Rate: {heart_rate} bpm, SpO2: {spo2}%")

            except Exception as e:
                print(f"Error reading data: {e}")

except serial.SerialException as e:
    print(f"Could not open serial port: {e}")

finally:
    if 'arduino' in locals() and arduino.is_open:
        arduino.close()
        print("Serial port closed.")
 