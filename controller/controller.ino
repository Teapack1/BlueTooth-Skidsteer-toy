#include <BluetoothSerial.h>

BluetoothSerial SerialBT;

#define FORWARD_BUTTON 32
#define REVERSE_BUTTON 33
#define LEFT_BUTTON 34
#define RIGHT_BUTTON 35
#define ARM_BUTTON 36

void setup() {
  pinMode(FORWARD_BUTTON, INPUT_PULLUP);
  pinMode(REVERSE_BUTTON, INPUT_PULLUP);
  pinMode(LEFT_BUTTON, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON, INPUT_PULLUP);
  pinMode(ARM_BUTTON, INPUT_PULLUP);
  
  SerialBT.begin("Controller"); 
  SerialBT.connect("Excavator"); // Connect to the excavator's Bluetooth
}

void loop() {
  int speed = 255; // You can modify this based on a potentiometer or other input
  if (digitalRead(FORWARD_BUTTON) == LOW) {
    SerialBT.print("F" + String(speed) + "\n");
    delay(100);
  }
  if (digitalRead(REVERSE_BUTTON) == LOW) {
    SerialBT.print("B" + String(speed) + "\n");
    delay(100);
  }
  if (digitalRead(LEFT_BUTTON) == LOW) {
    SerialBT.print("L" + String(speed) + "\n");
    delay(100);
  }
  if (digitalRead(RIGHT_BUTTON) == LOW) {
    SerialBT.print("R" + String(speed) + "\n");
    delay(100);
  }
  if (digitalRead(ARM_BUTTON) == LOW) {
    SerialBT.print("A" + String(speed) + "\n");
    delay(100);
  }
}
