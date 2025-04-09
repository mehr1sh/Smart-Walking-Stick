// Define ultrasonic sensor pins
const int trigFront = 18;
const int echoFront = 19;
const int trigSide  = 16;
const int echoSide  = 17;

// Buzzer pin
const int buzzerPin = 13;

// Threshold distance in cm
const int safeDistance = 30;

void setup() {
  Serial.begin(115200);

  // Set up ultrasonic sensors
  pinMode(trigFront, OUTPUT);
  pinMode(echoFront, INPUT);
  pinMode(trigSide, OUTPUT);
  pinMode(echoSide, INPUT);

  // Set up buzzer
  pinMode(buzzerPin, OUTPUT);
}

long getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2; // in cm
  return distance;
}

void loop() {
  long distanceFront = getDistance(trigFront, echoFront);
  long distanceSide  = getDistance(trigSide, echoSide);

  Serial.print("Front: ");
  Serial.print(distanceFront);
  Serial.print(" cm\tSide: ");
  Serial.print(distanceSide);
  Serial.println(" cm");

  if (distanceFront < safeDistance || distanceSide < safeDistance) {
    // Beep in intervals (e.g., 200 ms on, 200 ms off)
    digitalWrite(buzzerPin, HIGH);
    delay(200);
    digitalWrite(buzzerPin, LOW);
    delay(200);
  } else {
    // No obstacle → no beep
    digitalWrite(buzzerPin, LOW);
    delay(100);
  }
}
