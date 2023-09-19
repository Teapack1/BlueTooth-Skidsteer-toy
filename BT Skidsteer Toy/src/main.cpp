#include <BluetoothSerial.h>

BluetoothSerial SerialBT;

#define RIGHT_MOTOR_PIN1 25
#define RIGHT_MOTOR_PIN2 26
#define LEFT_MOTOR_PIN1 27
#define LEFT_MOTOR_PIN2 14
#define ARM_MOTOR_PIN1 12
#define ARM_MOTOR_PIN2 13

void moveForward(int speed) {
  analogWrite(RIGHT_MOTOR_PIN1, speed);
  analogWrite(LEFT_MOTOR_PIN1, speed);
}

void moveBackward(int speed) {
  analogWrite(RIGHT_MOTOR_PIN2, speed);
  analogWrite(LEFT_MOTOR_PIN2, speed);
}

void turnLeft(int speed) {
  analogWrite(RIGHT_MOTOR_PIN1, speed);
  analogWrite(LEFT_MOTOR_PIN2, speed);
}

void turnRight(int speed) {
  analogWrite(LEFT_MOTOR_PIN1, speed);
  analogWrite(RIGHT_MOTOR_PIN2, speed);
}

void moveArm(int speed) {
  analogWrite(ARM_MOTOR_PIN1, speed);
}

void stopAll() {
  analogWrite(RIGHT_MOTOR_PIN1, 0);
  analogWrite(RIGHT_MOTOR_PIN2, 0);
  analogWrite(LEFT_MOTOR_PIN1, 0);
  analogWrite(LEFT_MOTOR_PIN2, 0);
  analogWrite(ARM_MOTOR_PIN1, 0);
  analogWrite(ARM_MOTOR_PIN2, 0);
}

void setup() {
  pinMode(RIGHT_MOTOR_PIN1, OUTPUT);
  pinMode(RIGHT_MOTOR_PIN2, OUTPUT);
  pinMode(LEFT_MOTOR_PIN1, OUTPUT);
  pinMode(LEFT_MOTOR_PIN2, OUTPUT);
  pinMode(ARM_MOTOR_PIN1, OUTPUT);
  pinMode(ARM_MOTOR_PIN2, OUTPUT);
  
  SerialBT.begin("Excavator"); // Bluetooth device name
}

void loop() {
  if (SerialBT.available()) {
    String command = SerialBT.readStringUntil('\n');
    char action = command.charAt(0);
    int value = command.substring(1).toInt();
    switch (action) {
      case 'F': moveForward(value); break;
      case 'B': moveBackward(value); break;
      case 'L': turnLeft(value); break;
      case 'R': turnRight(value); break;
      case 'A': moveArm(value); break;
      case 'S': stopAll(); break;
    }
  }
}


