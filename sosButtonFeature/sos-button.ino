//jai shri ram
//jai shri krishna

#include <SoftwareSerial.h>
#include <TinyGPS++.h>

SoftwareSerial sim900(7, 8);  // GSM module (TX to D7, RX to D8)
SoftwareSerial gpsSerial(4, 3); // GPS module (TX to D4, RX to D3)
TinyGPSPlus gps;

const int buttonPin = 2; // SOS button connected to D2
bool buttonPressed = false;

void setup() {
    pinMode(buttonPin, INPUT_PULLUP); 
    Serial.begin(115200);
    sim900.begin(9600);
    gpsSerial.begin(9600);
    
    Serial.println("GSM & GPS Initialized...");
}

void sendSMS(String message) {
    Serial.println("Sending SOS...");
    sim900.println("AT+CMGF=1");  // Set SMS mode
    delay(500);
    sim900.println("AT+CMGS=\"+917204430421\""); // Replace with your phone number
    delay(500);
    sim900.print(message);  
    sim900.write(26);  // End SMS
    delay(1000);
    Serial.println("SOS Sent!");
}

String getGPSLocation() {
    while (gpsSerial.available()) {
        gps.encode(gpsSerial.read());
    }
    if (gps.location.isValid()) {
        String latitude = String(gps.location.lat(), 6);
        String longitude = String(gps.location.lng(), 6);
        return "Emergency! Location: https://maps.google.com/?q=" + latitude + "," + longitude;
    }
    return "Emergency! GPS not available.";
}

void loop() {
    if (digitalRead(buttonPin) == LOW && !buttonPressed) {
        Serial.println("SOS Button Pressed!");
        String gpsData = getGPSLocation();
        sendSMS(gpsData);
        buttonPressed = true;  // Prevent multiple triggers
        delay(5000);  // Debounce to prevent spam
    }
    
    if (digitalRead(buttonPin) == HIGH) { // Reset when button is released
        buttonPressed = false;
    }
}
