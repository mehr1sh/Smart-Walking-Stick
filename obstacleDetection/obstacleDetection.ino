// Defines pin numbers
// const int trigPin = 9;
// const int echoPin = 10;
// const int buzzer = 11;
// const int ledPin = 13;

// // Constants
// const int SAFE_DISTANCE = 5;  // Threshold distance in cm
// const int NUM_READINGS = 5;   // Number of readings for averaging

// void setup() {
//     pinMode(trigPin, OUTPUT);
//     pinMode(echoPin, INPUT);
//     pinMode(buzzer, OUTPUT);
//     pinMode(ledPin, OUTPUT);
//     Serial.begin(9600);
// }

// // Function to get an averaged distance for better stability
// int getDistance() {
//     long totalDuration = 0;
    
//     for (int i = 0; i < NUM_READINGS; i++) {
//         // Trigger the sensor
//         digitalWrite(trigPin, LOW);
//         delayMicroseconds(2);
//         digitalWrite(trigPin, HIGH);
//         delayMicroseconds(10);
//         digitalWrite(trigPin, LOW);
        
//         // Read the echo pin
//         totalDuration += pulseIn(echoPin, HIGH);
//         delay(10);  // Small delay to stabilize readings
//     }

//     long avgDuration = totalDuration / NUM_READINGS;
    
//     // Convert duration to distance (speed of sound = 343 m/s or 0.034 cm/μs)
//     return avgDuration * 0.034 / 2;
// }

// void loop() {
//     int distance = getDistance();
    
//     Serial.print("Distance: ");
//     Serial.print(distance);
//     Serial.println(" cm");

//     // Check if object is within the safety distance
//     if (distance <= SAFE_DISTANCE) {
//         digitalWrite(ledPin, HIGH);
        
//         // Make the buzzer beep instead of continuously staying on
//         tone(buzzer, 1000);  // 1kHz beep
//         delay(200);
//         noTone(buzzer);
//     } else {
//         digitalWrite(ledPin, LOW);
//         digitalWrite(buzzer, LOW);
//     }
    
//     delay(500);  // Delay to avoid excessive sensor polling
// }
    
    // The code is quite simple and self-explanatory. The  getDistance()  function is used to get an averaged distance reading from the ultrasonic sensor. The main loop checks if the distance is less than or equal to the safe distance (5 cm) and turns on the LED and buzzer accordingly. 
    // Upload the code to your Arduino board and test the obstacle detection system. 
    // Testing the Obstacle Detection System 
    // After uploading the code to the Arduino board, power it up using a USB cable or a 9V battery. 
    // The ultrasonic sensor will start measuring the distance between the sensor and the object in front of it. If the distance is less than or equal to 5 cm, the LED will turn on and the buzzer will beep. 
    // You can test the system by placing your hand or any object in front of the sensor. The LED and buzzer should turn on when the object is within the safe distance. 
    // Summary 
    // In this tutorial, we learned how to build an obstacle detection system using an ultrasonic sensor and an Arduino. We interfaced the HC-SR04 ultrasonic sensor with an Arduino board and wrote a simple code to detect obstacles within a safe distance. 
    // The system can be used in various applications such as robotics, security systems, and automation. You can further enhance the system by adding more sensors, a display, or a motor to avoid obstacles. 
    // If you have any questions or feedback, feel free to leave a comment below.



const int trig = 7;
const int echo = 8;
const int buzzer = 13;

void setup() {
    // put your setup code here, to run once:
    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);
    Serial.begin(9600);
}

void loop() {
    // put your main code here, to run repeatedly:
    digitalWrite(trig, LOW);
    delayMicroseconds(2);

    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    int duration = pulseIn(echo, HIGH);
    int distance = duration * (0.034 / 2);
    Serial.println(distance);

    if (distance < 15) 
        tone(buzzer, 500, 25);
    else 
        tone(buzzer, 0, 25);
    
    // delay(100);  // Uncomment if needed
}
