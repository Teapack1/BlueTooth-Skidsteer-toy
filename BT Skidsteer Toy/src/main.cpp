#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>

// Motor pins
#define LEFT_MOTOR_FWD 18
#define LEFT_MOTOR_REV 19
#define RIGHT_MOTOR_FWD 16
#define RIGHT_MOTOR_REV 17
#define EN1 25
#define EN2 26

// PWM channels
#define LEFT_MOTOR_CHANNEL 2
#define RIGHT_MOTOR_CHANNEL 3

// Servo pins
#define BUCKET_SERVO_PIN 22
#define ARM_SERVO_PIN 23

Servo bucketServo;
Servo armServo;

typedef struct struct_message {
  int joy1_x;
  int joy1_y;
  int joy2_x;
  int joy2_y;
} struct_message;

struct_message receivedData;

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len) {
  memcpy(&receivedData, incomingData, sizeof(receivedData));
  Serial.println("Data received.");
}

void controlMotor(int fwdPin, int revPin, int speed, int channel, int enablePin) {
  speed = constrain(speed, -255, 255);
  
  // Stop the motor first to ensure no conflicting signals
  digitalWrite(fwdPin, LOW);
  digitalWrite(revPin, LOW);
  
  if (speed > 0) {
    ledcWrite(channel, speed);
    digitalWrite(fwdPin, HIGH);
    digitalWrite(revPin, LOW);
  } else if (speed < 0) {
    ledcWrite(channel, abs(speed));
    digitalWrite(revPin, HIGH);
    digitalWrite(fwdPin, LOW);
  }
 
  analogWrite(enablePin, abs(speed)); // Control speed using enable pin
}


void setup() {
  Serial.begin(115200);
  
  pinMode(LEFT_MOTOR_FWD, OUTPUT);
  pinMode(LEFT_MOTOR_REV, OUTPUT); 
  pinMode(RIGHT_MOTOR_FWD, OUTPUT);
  pinMode(RIGHT_MOTOR_REV, OUTPUT);
  pinMode(EN1, OUTPUT);
  pinMode(EN2, OUTPUT);

  digitalWrite(LEFT_MOTOR_FWD, LOW);
  digitalWrite(RIGHT_MOTOR_FWD, LOW);
    digitalWrite(LEFT_MOTOR_REV, LOW);
  digitalWrite(RIGHT_MOTOR_REV, LOW);

  // configure LED PWM functionalitites
  ledcSetup(LEFT_MOTOR_CHANNEL, 800, 8);
  ledcSetup(RIGHT_MOTOR_CHANNEL, 800, 8);
    // attach the channel to the GPIO to be controlled
  ledcAttachPin(EN1, LEFT_MOTOR_CHANNEL);
  ledcAttachPin(EN2, LEFT_MOTOR_CHANNEL);

  
  bucketServo.attach(BUCKET_SERVO_PIN);
  armServo.attach(ARM_SERVO_PIN);

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  int leftMotorSpeed = receivedData.joy1_y;
  int rightMotorSpeed = -receivedData.joy1_y;

  if (receivedData.joy1_x > 0) {
    leftMotorSpeed -= receivedData.joy1_x;
    if (leftMotorSpeed <= -255) {
      rightMotorSpeed += (leftMotorSpeed + 255);
      leftMotorSpeed = -255;
    } if (receivedData.joy1_y > -10 && receivedData.joy1_y < 10) {
      rightMotorSpeed = leftMotorSpeed;
    }
    
  } else if (receivedData.joy1_x < 0) {
    rightMotorSpeed += abs(receivedData.joy1_x);
    if (rightMotorSpeed >= 255) {
      leftMotorSpeed += (rightMotorSpeed - 255);
      rightMotorSpeed = 255;
    } if (receivedData.joy1_y > -10 && receivedData.joy1_y < 10) {
      leftMotorSpeed = rightMotorSpeed;
    }
  }
  
  controlMotor(LEFT_MOTOR_FWD, LEFT_MOTOR_REV, leftMotorSpeed, LEFT_MOTOR_CHANNEL, EN1);
  controlMotor(RIGHT_MOTOR_FWD, RIGHT_MOTOR_REV, rightMotorSpeed, RIGHT_MOTOR_CHANNEL, EN2);

  bucketServo.write(map(receivedData.joy2_x, -255, 255, 0, 100));
  armServo.write(map(receivedData.joy2_y, -255, 255, 0, 100));

  Serial.println(receivedData.joy1_x);
  Serial.println(receivedData.joy1_y);
  Serial.println(receivedData.joy2_x);
  Serial.println(receivedData.joy2_y);
  Serial.println(leftMotorSpeed);
  Serial.println(rightMotorSpeed);

  delay(1000);
}
