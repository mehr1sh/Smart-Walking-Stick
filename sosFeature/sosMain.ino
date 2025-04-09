#include <WiFi.h>
#include "sos_notification.h"

const int buttonPin = 4;
bool sent = false;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(115200);
  WiFi.begin("Your_SSID", "Your_PASSWORD");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
}

void loop() {
  if (digitalRead(buttonPin) == LOW && !sent) {
    sendNotification();
    sent = true;
  }

  if (digitalRead(buttonPin) == HIGH) {
    sent = false;
  }
}
