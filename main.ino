#include <WiFi.h>
#include "ThingSpeak.h"
#include <HTTPClient.h>
#include <base64.h>
#include <TinyGPSPlus.h>

WiFiClient client;
unsigned long lastTSUpdate = 0;

// ThingSpeak settings
const char* thingSpeakAPIKey = "PADVNCIXK7HMZ89R";
unsigned long thingSpeakChannelNumber = 2949010;

// Ultrasonic sensor pins
const int trigA = 21, echoA = 19;
const int trigB = 22, echoB = 23;

// Buzzer pin
const int buzzer = 2;

// Detection thresholds
const int detectionThreshold = 100;
const int similarityThreshold = 10;

long getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2;
}

void beepLeft() {
  digitalWrite(buzzer, HIGH);
  delay(300);
  digitalWrite(buzzer, LOW);
  delay(700);
}

void beepRight() {
  digitalWrite(buzzer, HIGH);
  delay(200);
  digitalWrite(buzzer, LOW);
  delay(300);
}

void beepBoth() {
  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);
  delay(100);
}

void setupA() {
  Serial.begin(115200);
  pinMode(trigA, OUTPUT); pinMode(echoA, INPUT);
  pinMode(trigB, OUTPUT); pinMode(echoB, INPUT);
  pinMode(buzzer, OUTPUT);
}

void loopA() {
  long dA = getDistance(trigA, echoA);
  long dB = getDistance(trigB, echoB);

  bool leftDetected = dA < detectionThreshold;
  bool rightDetected = dB < detectionThreshold;
  bool bothSimilar = abs(dA - dB) < similarityThreshold;

  if (leftDetected && rightDetected && bothSimilar) {
    beepBoth();
  } else if (leftDetected) {
    beepLeft();
  } else if (rightDetected) {
    beepRight();
  } else {
    digitalWrite(buzzer, LOW);
    delay(200);
  }

  if (millis() - lastTSUpdate > 15000) {
    lastTSUpdate = millis();
    sendToThingSpeak(dA, dB, false);
  }
}

// GPS and SOS block
HardwareSerial gpsSerial(2);
TinyGPSPlus gps;

const char* ssid = "Niviboo";
const char* password = "Niviboo123";

const char* account_sid = "AC37ccc22d18b13400390fb2b009ebf1a4";
const char* auth_token  = "15fce4be715b5c005c900470bda7d1c5";
const char* from_number = "+17439626646";
const char* to_number   = "+917204430421";

const char* pb_token = "o.C2At451NNFQ35D5ES39FiKu9r9m8JM70";
const char* twiml_url = "https://handler.twilio.com/twiml/EH24ec49f5f6949ece3ae9423619572268";

const int buttonPin = 16;
const int ledPin = 18;

bool buttonPressed = false;

void setupB() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  gpsSerial.begin(9600, SERIAL_8N1, 4, 17);
  connectToWiFi();
  ThingSpeak.begin(client);
}

void loopB() {
  static unsigned long lastPressTime = 0;
  const unsigned long debounceDelay = 50;

  int reading = digitalRead(buttonPin);

  if (reading == LOW && !buttonPressed && millis() - lastPressTime > debounceDelay) {
    lastPressTime = millis();
    buttonPressed = true;
    digitalWrite(ledPin, HIGH);

    makeCall();
    sendPushbullet();

    delay(500);
    digitalWrite(ledPin, LOW);
  }

  if (reading == HIGH && buttonPressed) {
    buttonPressed = false;
  }
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void makeCall() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://api.twilio.com/2010-04-01/Accounts/" + String(account_sid) + "/Calls.json";
    String auth = base64::encode(String(account_sid) + ":" + String(auth_token));

    http.begin(url);
    http.addHeader("Authorization", "Basic " + auth);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String postData = "To=" + String(to_number) + "&From=" + String(from_number) + "&Url=" + String(twiml_url);
    http.POST(postData);
    http.end();
  }
}

String getGPSLocation() {
  unsigned long start = millis();
  while (millis() - start < 3000) {
    while (gpsSerial.available()) {
      gps.encode(gpsSerial.read());
    }
    if (gps.location.isUpdated()) {
      String lat = String(gps.location.lat(), 6);
      String lng = String(gps.location.lng(), 6);
      String link = "https://www.google.com/maps?q=" + lat + "," + lng;
      return "Location: " + link;
    }
  }
  return "GPS location not available.";
}

void sendPushbullet() {
  if (WiFi.status() == WL_CONNECTED) {
    String gpsMessage = getGPSLocation();

    HTTPClient http;
    http.begin("https://api.pushbullet.com/v2/pushes");
    http.addHeader("Access-Token", pb_token);
    http.addHeader("Content-Type", "application/json");

    String postData = "{\"type\": \"note\", \"title\": \"SOS Alert\", \"body\": \"The emergency button was pressed!\\n" + gpsMessage + "\"}";
    http.POST(postData);
    http.end();
  }

  sendToThingSpeak(0, 0, true);
}

void sendToThingSpeak(long dA, long dB, bool sosPressed) {
  ThingSpeak.setField(1, dA);
  ThingSpeak.setField(2, dB);
  ThingSpeak.setField(3, sosPressed ? 1 : 0);
  ThingSpeak.writeFields(thingSpeakChannelNumber, thingSpeakAPIKey);
}

// Combined setup and loop
void setup() {
  setupA();
  setupB();
}

void loop() {
  loopA();
  loopB();
}
