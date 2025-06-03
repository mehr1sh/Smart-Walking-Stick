import serial
import csv
import time

# Open serial port (adjust the port name based on your system)
ser = serial.Serial('COM3', 115200, timeout=1)  # Change 'COM3' to your port
time.sleep(2)  # Allow time for connection

# Open a CSV file to store the data
with open('ultrasonic_data.csv', 'w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(["Time", "Distance (cm)"])  # Header row

    try:
        while True:
            line = ser.readline().decode().strip()  # Read and decode data
            if line.startswith("Distance in CM: "):
                distance = line.split(":")[1].strip().split()[0]  # Extract value
                writer.writerow([time.strftime("%Y-%m-%d %H:%M:%S"), distance])
                print(f"Logged: {distance} cm")
    except KeyboardInterrupt:
        print("Logging stopped.")
        ser.close()
