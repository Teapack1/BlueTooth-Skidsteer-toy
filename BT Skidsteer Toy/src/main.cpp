#include <BluetoothSerial.h>

BluetoothSerial SerialBT;

// Motor pins
#define LEFT_MOTOR_FWD 25
#define LEFT_MOTOR_REV 26
#define RIGHT_MOTOR_FWD 27
#define RIGHT_MOTOR_REV 28
#define ARM_MOTOR_UP 29
#define ARM_MOTOR_DOWN 30
#define CLAW_MOTOR_OPEN 31
#define CLAW_MOTOR_CLOSE 32

void setup() {
  Serial.begin(115200);
  pinMode(LEFT_MOTOR_FWD, OUTPUT);
  pinMode(LEFT_MOTOR_REV, OUTPUT); 
  pinMode(RIGHT_MOTOR_FWD, OUTPUT);
  pinMode(RIGHT_MOTOR_REV, OUTPUT);
  pinMode(ARM_MOTOR_UP, OUTPUT);
  pinMode(ARM_MOTOR_DOWN, OUTPUT);
  pinMode(CLAW_MOTOR_OPEN, OUTPUT);
  pinMode(CLAW_MOTOR_CLOSE, OUTPUT);
  
  //Setup bluetooth wtih name "Excavator"
  SerialBT.begin("Excavator");
}

void loop() {

  if (SerialBT.available()) {
    String command = SerialBT.readStringUntil('\n');
    Serial.println(command);
    if (command.startsWith("M")) {
      // This line of code searches for the position (index) of the first occurrence of a comma , within the command string.
      int commaIndex = command.indexOf(',');
      //extracts a substring from the command string. This substring starts from the second character (index 1) and goes up to, but does not include, the character at the commaIndex position.
      int leftMotorSpeed = command.substring(1, commaIndex).toInt();
      //extracts a substring from the command string starting from the character immediately after the comma (hence commaIndex + 1) and goes until the end of the string.
      int rightMotorSpeed = command.substring(commaIndex + 1).toInt();
      Serial.print("Left motor speed: ");
      Serial.println(leftMotorSpeed);
      Serial.print("Right motor speed: ");
      Serial.println(rightMotorSpeed);

      //Control the motors:
      if (leftMotorSpeed > 0) {
        digitalWrite(LEFT_MOTOR_FWD, HIGH);
        digitalWrite(LEFT_MOTOR_REV, LOW);
      } else if (leftMotorSpeed < 0) {
        digitalWrite(LEFT_MOTOR_FWD, LOW);
        digitalWrite(LEFT_MOTOR_REV, HIGH);
      } else {
        digitalWrite(LEFT_MOTOR_FWD, LOW);
        digitalWrite(LEFT_MOTOR_REV, LOW);
      }
      if (rightMotorSpeed > 0) {
        digitalWrite(RIGHT_MOTOR_FWD, HIGH);
        digitalWrite(RIGHT_MOTOR_REV, LOW);
      } else if (rightMotorSpeed < 0) {
        digitalWrite(RIGHT_MOTOR_FWD, LOW);
        digitalWrite(RIGHT_MOTOR_REV, HIGH);
      } else {
        digitalWrite(RIGHT_MOTOR_FWD, LOW);
        digitalWrite(RIGHT_MOTOR_REV, LOW);
      }

      analogWrite(LEFT_MOTOR_FWD, abs(leftMotorSpeed));
      analogWrite(RIGHT_MOTOR_FWD, abs(rightMotorSpeed));

   
    } else if (command.startsWith("C")) {
      // Claw command
      int commaIndex = command.indexOf(',');
      int armMotorSpeed = command.substring(1, commaIndex).toInt();
      int clawMotorSpeed = command.substring(commaIndex + 1).toInt();
      Serial.print("Arm motor speed: ");
      Serial.println(armMotorSpeed);
      Serial.print("Claw motor speed: ");
      Serial.println(clawMotorSpeed);
      
      if (armMotorSpeed > 0) {
        digitalWrite(ARM_MOTOR_UP, HIGH);
        digitalWrite(ARM_MOTOR_DOWN, LOW);
      } else if (armMotorSpeed < 0) {
        digitalWrite(ARM_MOTOR_UP, LOW);
        digitalWrite(ARM_MOTOR_DOWN, HIGH);
      } else {
        digitalWrite(ARM_MOTOR_UP, LOW);
        digitalWrite(ARM_MOTOR_DOWN, LOW);
      }
      if (clawMotorSpeed > 0) {
        digitalWrite(CLAW_MOTOR_OPEN, HIGH);
        digitalWrite(CLAW_MOTOR_CLOSE, LOW);
      } else if (clawMotorSpeed < 0) {
        digitalWrite(CLAW_MOTOR_OPEN, LOW);
        digitalWrite(CLAW_MOTOR_CLOSE, HIGH);
      } else {
        digitalWrite(CLAW_MOTOR_OPEN, LOW);
        digitalWrite(CLAW_MOTOR_CLOSE, LOW);
      }
  }
  }
}