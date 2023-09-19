#include <BluetoothSerial.h>

BluetoothSerial SerialBT;

// Movement Joystick
#define JOY1_X 32
#define JOY1_Y 33

// Claw Joystick
#define JOY2_X 34
#define JOY2_Y 35
#define JOY2_Z 36  // Assuming a button for the Z axis

void setup() {
  Serial.begin(115200);
  delay(10);
  SerialBT.begin("ESP32Controller");  // Bluetooth device name
  SerialBT.connect("Excavator");  // Bluetooth device name
  
}

void loop() {
  //Read the joystick values
  int joy1_x_value = analogRead(JOY1_X);
  int joy1_y_value = analogRead(JOY1_Y);
  int joy2_x_value = analogRead(JOY2_X);
  int joy2_y_value = analogRead(JOY2_Y);
  int joy2_z_value = digitalRead(JOY2_Z);

  //map joystick valuest for the motors
  int mappedJoy1X = map(joy1_x_value, 0, 4095, -255, 255);
  int mappedJoy1Y = map(joy1_y_value, 0, 4095, -255, 255);
  int mappedJoy2X = map(joy2_x_value, 0, 4095, -255, 255);
  int mappedJoy2Y = map(joy2_y_value, 0, 4095, -255, 255);

  // Send the values over BT to excavator.
  SerialBT.print("M" + String(mappedJoy1X) + "," + String(mappedJoy1Y) + "\n"); //Movement values
  SerialBT.print("C" + String(mappedJoy2X) + "," + String(mappedJoy2Y) + "," + String(joy2_z_value) + "\n"); //Claw values

  //write the values to serial monitor for debugging
  Serial.print("M" + String(mappedJoy1X) + "," + String(mappedJoy1Y) + "\n"); //Movement values
  Serial.print("C" + String(mappedJoy2X) + "," + String(mappedJoy2Y) + "," + String(joy2_z_value) + "\n"); //Claw values

delay(500);
}

