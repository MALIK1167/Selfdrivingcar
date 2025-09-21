// Self-Driving Car Powered by Arduino
// Project by Malik

#include <Servo.h>

// Pin Definitions
const int SERVO_PIN = 6;
const int SENSOR_FRONT = A0;
const int SENSOR_LEFT  = A1;
const int SENSOR_RIGHT = A2;
const int MOTOR_LEFT_FWD  = 9;
const int MOTOR_LEFT_BWD  = 10;
const int MOTOR_RIGHT_FWD = 11;
const int MOTOR_RIGHT_BWD = 12;

// Thresholds and Timing
const int OBSTACLE_THRESHOLD = 300;
const int SERVO_CENTER = 90;
const int SERVO_LEFT = 0;
const int SERVO_RIGHT = 180;
const int TURN_DURATION = 900; // milliseconds
const int SCAN_DELAY = 800;    // milliseconds

Servo headServo;

void setup() {
  Serial.begin(9600);
  headServo.attach(SERVO_PIN);

  pinMode(SENSOR_FRONT, INPUT);
  pinMode(SENSOR_LEFT, INPUT);
  pinMode(SENSOR_RIGHT, INPUT);

  pinMode(MOTOR_LEFT_FWD, OUTPUT);
  pinMode(MOTOR_LEFT_BWD, OUTPUT);
  pinMode(MOTOR_RIGHT_FWD, OUTPUT);
  pinMode(MOTOR_RIGHT_BWD, OUTPUT);

  headServo.write(SERVO_CENTER);
  stopAllMotors();
}

void loop() {
  int frontVal = analogRead(SENSOR_FRONT);
  Serial.print("Front Sensor: ");
  Serial.println(frontVal);

  if (frontVal < OBSTACLE_THRESHOLD) {
    stopAllMotors();
    delay(250);

    // Scan Environment
    int leftVal  = scanDirection(SERVO_LEFT, SENSOR_LEFT);
    int rightVal = scanDirection(SERVO_RIGHT, SENSOR_RIGHT);

    headServo.write(SERVO_CENTER);
    delay(300);

    // Decide best direction
    if (leftVal > rightVal) {
      turnLeft();
    } else {
      turnRight();
    }
  } else {
    driveForward();
  }
  delay(60);
}

int scanDirection(int servoAngle, int sensorPin) {
  headServo.write(servoAngle);
  delay(SCAN_DELAY);
  int val = analogRead(sensorPin);
  Serial.print("Sensor at angle ");
  Serial.print(servoAngle);
  Serial.print(": ");
  Serial.println(val);
  return val;
}

void driveForward() {
  setMotor(MOTOR_LEFT_FWD,  HIGH);
  setMotor(MOTOR_LEFT_BWD,  LOW);
  setMotor(MOTOR_RIGHT_FWD, HIGH);
  setMotor(MOTOR_RIGHT_BWD, LOW);
}

void turnLeft() {
  setMotor(MOTOR_LEFT_FWD,  LOW);
  setMotor(MOTOR_LEFT_BWD,  HIGH);
  setMotor(MOTOR_RIGHT_FWD, HIGH);
  setMotor(MOTOR_RIGHT_BWD, LOW);
  delay(TURN_DURATION);
  stopAllMotors();
}

void turnRight() {
  setMotor(MOTOR_LEFT_FWD,  HIGH);
  setMotor(MOTOR_LEFT_BWD,  LOW);
  setMotor(MOTOR_RIGHT_FWD, LOW);
  setMotor(MOTOR_RIGHT_BWD, HIGH);
  delay(TURN_DURATION);
  stopAllMotors();
}

void stopAllMotors() {
  setMotor(MOTOR_LEFT_FWD,  LOW);
  setMotor(MOTOR_LEFT_BWD,  LOW);
  setMotor(MOTOR_RIGHT_FWD, LOW);
  setMotor(MOTOR_RIGHT_BWD, LOW);
}

void setMotor(int pin, int state) {
  digitalWrite(pin, state);
}
