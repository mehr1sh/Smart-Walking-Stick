// Ultrasonic sensor pins
const int trigA = 21, echoA = 19; // Left sensor
const int trigB = 22, echoB = 23; // Right sensor

// Buzzer pin
const int buzzer = 4;

// Detection settings
const int detectionThreshold = 100;      // cm
const int similarityThreshold = 10;      // cm difference for same object

void setup() {
  Serial.begin(9600);
  pinMode(trigA, OUTPUT); pinMode(echoA, INPUT);
  pinMode(trigB, OUTPUT); pinMode(echoB, INPUT);
  pinMode(buzzer, OUTPUT);
}

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

void loop() {
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
