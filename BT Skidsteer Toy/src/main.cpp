#include <BluetoothSerial.h>
#include <ESP32Servo.h>

BluetoothSerial SerialBT;

// Motor pins
#define LEFT_MOTOR_FWD 25
#define LEFT_MOTOR_REV 26
#define RIGHT_MOTOR_FWD 27
#define RIGHT_MOTOR_REV 32

// Servo pins
#define ARM_SERVO_PIN 33
#define CLAW_SERVO_PIN 34

//#define USE_PIN // Uncomment this to use PIN during pairing. The pin is specified on the line below
const char *pin = "1234"; // Change this to more secure PIN.

String device_name = "Excavator";

void controlMotor(int, int, int);

Servo armServo;
Servo clawServo;

void setup() {
  Serial.begin(115200);
  
  // Initialize motor pins
  pinMode(LEFT_MOTOR_FWD, OUTPUT);
  pinMode(LEFT_MOTOR_REV, OUTPUT); 
  pinMode(RIGHT_MOTOR_FWD, OUTPUT);
  pinMode(RIGHT_MOTOR_REV, OUTPUT);
  
  // Attach servos
  armServo.attach(ARM_SERVO_PIN);
  clawServo.attach(CLAW_SERVO_PIN);
  
  // Setup Bluetooth with name "Excavator"
  SerialBT.begin(device_name); //Bluetooth device name
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());
  
  #ifdef USE_PIN
    SerialBT.setPin(pin);
    Serial.println("Using PIN");
  #endif
}

void loop() {
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    String command = SerialBT.readStringUntil('\n');
    Serial.println(command);
    
    if (command.startsWith("M")) {
      int commaIndex = command.indexOf(',');
      int leftMotorSpeed = command.substring(1, commaIndex).toInt();
      int rightMotorSpeed = command.substring(commaIndex + 1).toInt();

      // Control the wheel motors
      controlMotor(LEFT_MOTOR_FWD, LEFT_MOTOR_REV, leftMotorSpeed);
      controlMotor(RIGHT_MOTOR_FWD, RIGHT_MOTOR_REV, rightMotorSpeed);
      
    } else if (command.startsWith("C")) {
      int commaIndex = command.indexOf(',');
      int armPosition = command.substring(1, commaIndex).toInt();
      int clawPosition = command.substring(commaIndex + 1, commaIndex + 4).toInt(); // Adjusted to read the next value correctly

      // Ensure the servo values are within the valid range
      armPosition = constrain(armPosition, 0, 180);
      clawPosition = constrain(clawPosition, 0, 180);

      // Control the servos
      armServo.write(armPosition);
      clawServo.write(clawPosition);
    } else {
      SerialBT.println("Invalid command received.");
    }
  }
}

void controlMotor(int fwdPin, int revPin, int speed) {
  if (speed > 0) {
    analogWrite(fwdPin, speed);
    digitalWrite(revPin, LOW);
  } else if (speed < 0) {
    analogWrite(revPin, abs(speed));
    digitalWrite(fwdPin, LOW);
  } else {
    digitalWrite(fwdPin, LOW);
    digitalWrite(revPin, LOW);
  }
}
