| ESP32 Pin | Component              | Description            |
|-----------|------------------------|------------------------|
| 21        | Ultrasonic Trig (Left) | Output to sensor       |
| 19        | Ultrasonic Echo (Left) | Input from sensor      |
| 22        | Ultrasonic Trig (Right)| Output to sensor       |
| 23        | Ultrasonic Echo (Right)| Input from sensor      |
| 2         | Buzzer                 | Output for alerts      |
| 16        | SOS Button             | Input, active low      |
| 18        | LED Indicator          | Output, SOS feedback   |
| 4         | GPS TX                 | GPS data to ESP32      |
| 17        | GPS RX                 | (optional, rarely used)|


# Wiring Diagram Description

This document details the wiring connections for the Smart Walking Stick project using an ESP32, two ultrasonic sensors, a buzzer, an SOS button, an LED indicator, and a NEO-6M GPS module.

---

## Ultrasonic Sensor 1 (Left)
- **Trig** → ESP32 **GPIO 21**
- **Echo** → ESP32 **GPIO 19**

## Ultrasonic Sensor 2 (Right)
- **Trig** → ESP32 **GPIO 22**
- **Echo** → ESP32 **GPIO 23**

## Buzzer
- **Positive** → ESP32 **GPIO 2**
- **Negative** → **GND**

## SOS Button
- **One leg** → ESP32 **GPIO 16** (with internal pull-up)
- **Other leg** → **GND**

## LED Indicator
- **Anode** → ESP32 **GPIO 18** (with current-limiting resistor)
- **Cathode** → **GND**

## NEO-6M GPS Module
- **VCC** → **3.3V** or **5V** on ESP32 (module accepts both, but 3.3V is safest for ESP32)
- **GND** → **GND**
- **TX** → ESP32 **GPIO 4** (GPS data out to ESP32)
- **RX** → ESP32 **GPIO 17** (GPS data in from ESP32, rarely used, can be left unconnected if not needed)

---

**Note:**  
- Always use a current-limiting resistor with the LED to prevent damage.
- If using the GPS RX pin, ensure voltage compatibility (ESP32 is 3.3V logic).
- Double-check all connections before powering the circuit.
