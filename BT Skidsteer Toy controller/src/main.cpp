#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

// Movement Joystick
#define JOY1_X 32
#define JOY1_Y 33

// Claw Joystick
#define JOY2_X 34
#define JOY2_Y 35
#define JOY2_Z 36  // Assuming a button for the Z axis

bool isConnected = false;

#define USE_NAME // Comment this to use MAC address instead of a slaveName
const char *pin = "1234"; // Change this to reflect the pin expected by the real slave BT device

#ifdef USE_NAME
  String slaveName = "ESP32-BT-Slave"; // Change this to reflect the real name of your slave BT device
#else
  String MACadd = "AA:BB:CC:11:22:33"; // This only for printing
  uint8_t address[6]  = {0xAA, 0xBB, 0xCC, 0x11, 0x22, 0x33}; // Change this to reflect real MAC address of your slave BT device
#endif

String myName = "ESP32-BT-Master";

void connectToExcavator() {
  if (!isConnected) {
    Serial.println("Attempting to connect to Excavator...");
    #ifdef USE_NAME
      isConnected = SerialBT.connect(slaveName);
    #else
      isConnected = SerialBT.connect(address);
    #endif
    if (isConnected) {
      Serial.println("Connected to Excavator!");
    } else {
      Serial.println("Failed to connect. Will retry...");
      delay(2000);  // Give it some time before the next attempt
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);  // Delay for stabilization
  
  SerialBT.begin(myName, true);
  Serial.printf("The device \"%s\" started in master mode, make sure slave BT device is on!\n", myName.c_str());
  
  #ifndef USE_NAME
    SerialBT.setPin(pin);
    Serial.println("Using PIN");
  #endif

  connectToExcavator();
}

void loop() {
  // Check Bluetooth connection and attempt to reconnect if lost
  connectToExcavator();

  // Read the joystick values
  int joy1_x_value = analogRead(JOY1_X);
  int joy1_y_value = analogRead(JOY1_Y);
  int joy2_x_value = analogRead(JOY2_X);
  int joy2_y_value = analogRead(JOY2_Y);
  int joy2_z_value = digitalRead(JOY2_Z);

  // Map joystick values for the motors
  int mappedJoy1X = map(joy1_x_value, 0, 4095, -255, 255);
  int mappedJoy1Y = map(joy1_y_value, 0, 4095, -255, 255);
  int mappedJoy2X = map(joy2_x_value, 0, 4095, -255, 255);
  int mappedJoy2Y = map(joy2_y_value, 0, 4095, -255, 255);

  // Send the values over BT to excavator.
  if (SerialBT.available()) {
    SerialBT.print("M" + String(mappedJoy1X) + "," + String(mappedJoy1Y) + "\n"); // Movement values
    SerialBT.print("C" + String(mappedJoy2X) + "," + String(mappedJoy2Y) + "," + String(joy2_z_value) + "\n"); // Claw values
  } else {
    isConnected = false;
  }

  // Write the values to serial monitor for debugging
  Serial.print("M" + String(mappedJoy1X) + "," + String(mappedJoy1Y) + "\n"); // Movement values
  Serial.print("C" + String(mappedJoy2X) + "," + String(mappedJoy2Y) + "," + String(joy2_z_value) + "\n"); // Claw values

  delay(500);
}
