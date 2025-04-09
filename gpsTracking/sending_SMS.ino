void setup() {
  Serial2.begin(9600, SERIAL_8N1, 16, 17); // RX=16, TX=17
  delay(1000);
  sendSMS();
}

void sendSMS() {
  Serial2.println("AT+CMGF=1"); // SMS mode
  delay(1000);
  Serial2.println("AT+CMGS=\"+91XXXXXXXXXX\""); // replace with your number
  delay(1000);
  Serial2.println("Hello from ESP32!");
  delay(500);
  Serial2.write(26); // Ctrl+Z to send
}

void loop() {
}
