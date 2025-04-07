// Define pins for Sensor 1
const int trigPin1 = 5;
const int echoPin1 = 18;

// Define pins for Sensor 2
const int trigPin2 = 17;
const int echoPin2 = 16;

// Buzzer pin
const int buzzerPin = 19;

// Threshold distance in cm
const int SAFE_DISTANCE = 15;

// Number of readings for averaging
const int NUM_READINGS = 5;

void setup() {
  Serial.begin(115200);

  // Set up pin modes for both sensors
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);

  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  // Set up buzzer
  pinMode(buzzerPin, OUTPUT);
}

long getAverageDistance(int trigPin, int echoPin) {
  long total = 0;
  for (int i = 0; i < NUM_READINGS; i++) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH, 20000); // 20ms timeout
    total += duration;
    delay(10);
  }
  long avgDuration = total / NUM_READINGS;
  long distance = avgDuration * 0.034 / 2;
  return distance;
}

void loop() {
  long distance1 = getAverageDistance(trigPin1, echoPin1);
  long distance2 = getAverageDistance(trigPin2, echoPin2);

  Serial.print("Distance1: ");
  Serial.print(distance1);
  Serial.print(" cm, Distance2: ");
  Serial.println(distance2);

  // If either sensor detects something too close, buzz
  if (distance1 <= SAFE_DISTANCE || distance2 <= SAFE_DISTANCE) {
    tone(buzzerPin, 1000); // 1kHz sound
    delay(200);
    noTone(buzzerPin);
  } else {
    noTone(buzzerPin);
  }

  delay(300); // Control polling frequency
}
