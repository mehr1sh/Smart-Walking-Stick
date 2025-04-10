#include <WiFi.h>
#include <HTTPClient.h>
#include <base64.h>

// WiFi credentials
const char* ssid = "Niviboo";
const char* password = "Niviboo123";

// Twilio Credentials
const char* account_sid = "AC37ccc22d18b13400390fb2b009ebf1a4";        // 🔁 Replace
const char* auth_token  = "15fce4be715b5c005c900470bda7d1c5"; // 🔁 Replace
const char* from_number = "+17439626646";           // 🔁 Your Twilio number
const char* to_number   = "+917204430421";          // 🔁 Your number

// Pushbullet Token
const char* pb_token = "o.LIkpGIQncVDbnEovMwzXs31w44Xg1O9g"; // 🔁 Replace

// TwiML Bin URL
const char* twiml_url = "https://handler.twilio.com/twiml/EH24ec49f5f6949ece3ae9423619572268";

// Pins
const int buttonPin = 16;
const int ledPin = 18;

bool buttonPressed = false;

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  connectToWiFi();
}

void loop() {
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
