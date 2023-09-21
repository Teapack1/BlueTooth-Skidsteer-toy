#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>

// Motor pins
#define LEFT_MOTOR_FWD 33
#define LEFT_MOTOR_REV 26
#define RIGHT_MOTOR_FWD 27
#define RIGHT_MOTOR_REV 32

// Servo pins
#define ARM_SERVO_PIN 25
#define CLAW_SERVO_PIN 34

Servo armServo;
Servo clawServo;

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
  
  // Attach servos
  armServo.attach(ARM_SERVO_PIN);
  clawServo.attach(CLAW_SERVO_PIN);

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
  // Control the wheel motors
  controlMotor(LEFT_MOTOR_FWD, LEFT_MOTOR_REV, receivedData.joy1_x);
  controlMotor(RIGHT_MOTOR_FWD, RIGHT_MOTOR_REV, receivedData.joy1_y);

  // Control the servos
  armServo.write(map(receivedData.joy2_x, -255, 255, 0, 180));
  clawServo.write(map(receivedData.joy2_y, -255, 255, 0, 180));
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
