// --------------------- CODE BLOCK A ---------------------
// Ultrasonic sensor pins
const int trigA = 21, echoA = 19; // Left sensor
const int trigB = 22, echoB = 23; // Right sensor

// Buzzer pin
const int buzzer = 4;

// Detection settings
const int detectionThreshold = 100;      // cm
const int similarityThreshold = 10;      // cm difference for same object

long getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2;
}

void beepSlow() {
  Serial.println("Beeping slowly - small object detected.");
  digitalWrite(buzzer, HIGH);
  delay(200);
  digitalWrite(buzzer, LOW);
  delay(800);
}

void beepFast() {
  Serial.println("Beeping fast - large object detected.");
  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);
  delay(100);
}

void setupA() {
  Serial.begin(9600);
  pinMode(trigA, OUTPUT); pinMode(echoA, INPUT);
  pinMode(trigB, OUTPUT); pinMode(echoB, INPUT);
  pinMode(buzzer, OUTPUT);
}

void loopA() {
  long dA = getDistance(trigA, echoA);
  long dB = getDistance(trigB, echoB);

  Serial.print("Left: "); Serial.print(dA); Serial.print(" cm | ");
  Serial.print("Right: "); Serial.print(dB); Serial.println(" cm");

  bool leftDetected = dA < detectionThreshold;
  bool rightDetected = dB < detectionThreshold;
  bool bothSimilar = abs(dA - dB) < similarityThreshold;

  if (leftDetected && rightDetected && bothSimilar) {
    beepFast(); // Large object detected
  } else if (leftDetected || rightDetected) {
    beepSlow(); // Small object detected
  } else {
    digitalWrite(buzzer, LOW); // No object
    Serial.println("No object detected - buzzer off.");
    delay(200);
  }
}

// --------------------- CODE BLOCK B ---------------------
#include <WiFi.h>
#include <HTTPClient.h>
#include <base64.h>

// WiFi credentials
const char* ssid = "Niviboo";
const char* password = "Niviboo123";

// Twilio Credentials
const char* account_sid = "AC37ccc22d18b13400390fb2b009ebf1a4";
const char* auth_token  = "15fce4be715b5c005c900470bda7d1c5";
const char* from_number = "+17439626646";
const char* to_number   = "+917204430421";

// Pushbullet Token
const char* pb_token = "o.LIkpGIQncVDbnEovMwzXs31w44Xg1O9g";

// TwiML Bin URL
const char* twiml_url = "https://handler.twilio.com/twiml/EH24ec49f5f6949ece3ae9423619572268";

// Pins
const int buttonPin = 16;
const int ledPin = 18;

bool buttonPressed = false;

void setupB() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  connectToWiFi();
}

void loopB() {
  static unsigned long lastPressTime = 0;
  const unsigned long debounceDelay = 50;

  int reading = digitalRead(buttonPin);

  if (reading == LOW && !buttonPressed && millis() - lastPressTime > debounceDelay) {
    lastPressTime = millis();
    buttonPressed = true;
    Serial.println("🔘 Button Pressed!");
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
  Serial.print("📶 Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi Connected");
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
    int httpCode = http.POST(postData);

    Serial.print("Twilio call response: ");
    Serial.println(httpCode);

    http.end();
  } else {
    Serial.println("Not connected to WiFi");
  }
}

void sendPushbullet() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("https://api.pushbullet.com/v2/pushes");
    http.addHeader("Access-Token", pb_token);
    http.addHeader("Content-Type", "application/json");

    String postData = "{\"type\": \"note\", \"title\": \"🚨 SOS Alert!\", \"body\": \"The emergency button was pressed!\"}";
    int httpCode = http.POST(postData);

    Serial.print("📲 Pushbullet response: ");
    Serial.println(httpCode);

    http.end();
  } else {
    Serial.println(" Not connected to WiFi");
  }
}

// --------------------- UNIFIED ENTRY POINT ---------------------
void setup() {
  setupA();
  setupB();
}

void loop() {
  loopA();
  loopB();
}
