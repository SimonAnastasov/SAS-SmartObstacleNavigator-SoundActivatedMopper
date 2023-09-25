#include <Servo.h>

// Globals
Servo SERVO;

int doWork = false;

int rotating = false;

int rotatingIterations = 0;
int rotatingIterationsLimit = 160;

int distance = 1000;

// Pins
const int MOTOR_A_PIN_1 = 2;
const int MOTOR_A_PIN_2 = 3;
const int MOTOR_B_PIN_1 = 4;
const int MOTOR_B_PIN_2 = 5;

const int SERVO_PIN = 6;
const int MICROPHONE_PIN = 9;

const int ULTRASONIC_PIN_TRIG = 10;
const int ULTRASONIC_PIN_ECHO = 11;


// Setup
void setup() {
  SERVO.attach(SERVO_PIN);
  pinMode(MICROPHONE_PIN, INPUT);

  pinMode(MOTOR_A_PIN_1, OUTPUT);
  pinMode(MOTOR_A_PIN_2, OUTPUT);
  pinMode(MOTOR_B_PIN_1, OUTPUT);
  pinMode(MOTOR_B_PIN_2, OUTPUT);

  pinMode(ULTRASONIC_PIN_TRIG, OUTPUT);
  pinMode(ULTRASONIC_PIN_ECHO, INPUT);

  Serial.begin(9600);
}

void printData() {
  if (!doWork) {
    Serial.println("ON PAUSE!");
    return ;
  }

  Serial.print("ROTATING: ");
  Serial.println(rotating);

  Serial.print("ROTATING ITERATIONS: ");
  Serial.println(rotatingIterations);

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
}

// Loop
void loop() {
  // Force stop;
  // return;

  // Print function;
  printData();

  // Notice sound (uses MICROPHONE);
  noticeSound();

  // Clean (uses SERVO);
  clean();

  // Get distance (uses ULTRASONIC);
  getDistance();


  if (distance < 25) {
    rotating = true;
  }

  // Move forward or rotate (uses MOTOR);
  move();
}

void getDistance() {
  if (rotating) {
    return ;
  }

  digitalWrite(ULTRASONIC_PIN_TRIG, LOW);
  delayMicroseconds(2);
  
  digitalWrite(ULTRASONIC_PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_PIN_TRIG, LOW);
  
  unsigned long duration = pulseIn(ULTRASONIC_PIN_ECHO, HIGH);
  
  // Calculate distance in centimeters
  distance = duration / 58.2;
  
  delay(100);  // Delay before taking the next measurement
}

void move() {
  if (!doWork) {
    // Stop Motor A
    digitalWrite(MOTOR_A_PIN_1, LOW);
    digitalWrite(MOTOR_A_PIN_2, LOW);

    // Stop Motor B
    digitalWrite(MOTOR_B_PIN_1, LOW);
    digitalWrite(MOTOR_B_PIN_2, LOW);

    return ;
  }

  // If not rotating, move forward;
  if (!rotating) {
    // Motor A forward
    digitalWrite(MOTOR_A_PIN_1, LOW);
    digitalWrite(MOTOR_A_PIN_2, HIGH);

    // Motor B forward
    digitalWrite(MOTOR_B_PIN_1, LOW);
    digitalWrite(MOTOR_B_PIN_2, HIGH);
  }
  // Else, rotate it,
  else {
    if (rotatingIterations > rotatingIterationsLimit) {
      rotatingIterations = 0;
      rotating = false;

      return ;
    }

    // Motor A forward
    digitalWrite(MOTOR_A_PIN_1, LOW);
    digitalWrite(MOTOR_A_PIN_2, HIGH);

    // Motor B backward
    digitalWrite(MOTOR_B_PIN_1, HIGH);
    digitalWrite(MOTOR_B_PIN_2, LOW);

    rotatingIterations++;
  }
}

// Functions
void clean() {
  if (!doWork) {
    return ;
  }

  SERVO.write(135);
  delay(100);
  SERVO.write(170);
  delay(100);
}

void noticeSound() {
  int microphoneData = digitalRead(MICROPHONE_PIN);

  if (microphoneData == HIGH) {
    doWork = !doWork;

    delay(300);
  }
}

