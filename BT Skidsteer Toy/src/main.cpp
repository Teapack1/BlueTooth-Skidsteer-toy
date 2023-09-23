#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>

// Motor pins
#define LEFT_MOTOR_FWD 16
#define LEFT_MOTOR_REV 17
#define RIGHT_MOTOR_FWD 18
#define RIGHT_MOTOR_REV 19
#define THIRD_MOTOR_FWD 12
#define THIRD_MOTOR_REV 14

// Servo pins
#define ARM_SERVO_PIN 32

void controlMotor(int, int, int);

Servo armServo;

// Define a data structure
typedef struct struct_message {
  int joy1_x;
  int joy1_y;
  int joy2_x;
  int joy2_y;
  int joy2_z;
} struct_message;

// Create a structured object
struct_message receivedData;

// Callback function to receive data
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len) {
  memcpy(&receivedData, incomingData, sizeof(receivedData));
  Serial.println("Data received.");
}

void setup() {
  Serial.begin(115200);
  
  // Initialize motor pins
  pinMode(LEFT_MOTOR_FWD, OUTPUT);
  pinMode(LEFT_MOTOR_REV, OUTPUT); 
  pinMode(RIGHT_MOTOR_FWD, OUTPUT);
  pinMode(RIGHT_MOTOR_REV, OUTPUT);
  pinMode(THIRD_MOTOR_FWD, OUTPUT);
  pinMode(THIRD_MOTOR_REV, OUTPUT);
  
  // Attach servo
  armServo.attach(ARM_SERVO_PIN);

  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the receive callback
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  int leftMotorSpeed = receivedData.joy1_x;
  int rightMotorSpeed = receivedData.joy1_x;

  // Adjust speeds for turning based on joy1_y
  if (receivedData.joy1_y > 0) {
    leftMotorSpeed += receivedData.joy1_y; // Increase speed of left motor to turn right
    if (leftMotorSpeed > 255) {
      rightMotorSpeed -= (leftMotorSpeed - 255); // Subtract excess from right motor
      leftMotorSpeed = 255; // Cap left motor speed
    }
  } else {
    rightMotorSpeed += abs(receivedData.joy1_y); // Increase speed of right motor to turn left
    if (rightMotorSpeed > 255) {
      leftMotorSpeed -= (rightMotorSpeed - 255); // Subtract excess from left motor
      rightMotorSpeed = 255; // Cap right motor speed
    }
  }

  Serial.println("left" + leftMotorSpeed);
  Serial.println("right" + rightMotorSpeed);
  
  // Control the wheel motors
  controlMotor(LEFT_MOTOR_FWD, LEFT_MOTOR_REV, leftMotorSpeed);
  controlMotor(RIGHT_MOTOR_FWD, RIGHT_MOTOR_REV, rightMotorSpeed);
  controlMotor(THIRD_MOTOR_FWD, THIRD_MOTOR_REV, receivedData.joy2_y); // Control third motor

  // Control the servo
  armServo.write(map(receivedData.joy2_x, -255, 255, 0, 180));
}

void controlMotor(int fwdPin, int revPin, int speed) {
  if (speed > 255) speed = 255; // Cap the speed
  if (speed < -255) speed = -255; // Cap the speed
  
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
