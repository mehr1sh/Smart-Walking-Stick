#define TRIGGER_PIN 5  // Example pin for trigger
#define ECHO_PIN 18     // Example pin for echo

// Speed of sound in cm/microsecond
const float SOUND_SPEED = 0.034;

void setup() {
  // Start serial communication
  Serial.begin(115200);
  
  // Set trigger pin as output, echo pin as input
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  // Clear the trigger pin
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  
  // Send 10 microsecond pulse
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  
  // Measure echo duration
  long duration = pulseIn(ECHO_PIN, HIGH);
  
  // Calculate distance in different units
  float distanceCm = duration * SOUND_SPEED / 2;
  
  // Print distances
  Serial.print("Distance in CM: ");
  Serial.print(distanceCm);
  Serial.println(" in");
  
  delay(1000);  // Wait for a second before next reading
}
