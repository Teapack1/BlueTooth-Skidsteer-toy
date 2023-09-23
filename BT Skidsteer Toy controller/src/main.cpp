#include <esp_now.h>
#include <WiFi.h>

// Movement Joystick
#define JOY1_X 39
#define JOY1_Y 36

// Claw Joystick
#define JOY2_X 32
#define JOY2_Y 33
#define JOY2_Z 25  // Assuming a button for the Z axis

// MAC Address of the toy - edit as required
uint8_t toyAddress[] = {0x78, 0xE3, 0x6D, 0x0A, 0x01, 0x60};

// Define a data structure
typedef struct struct_message {
  int joy1_x;
  int joy1_y;
  int joy2_x;
  int joy2_y;
  int joy2_z;
} struct_message;

// Create a structured object
struct_message myData;

// Peer info
esp_now_peer_info_t peerInfo;

// Callback function called when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  // Set up Serial Monitor
  Serial.begin(115200);
  delay(1000);  // Delay for stabilization

  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the send callback
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, toyAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // Read the joystick values
  int joy1_x_value = analogRead(JOY1_X);
  int joy1_y_value = analogRead(JOY1_Y);
  int joy2_x_value = analogRead(JOY2_X);
  int joy2_y_value = analogRead(JOY2_Y);
  int joy2_z_value = digitalRead(JOY2_Z);

  // Map joystick values for the motors
  myData.joy1_x = map(joy1_x_value, 0, 4095, -255, 255);
  myData.joy1_y = map(joy1_y_value, 0, 4095, -255, 255);
  myData.joy2_x = map(joy2_x_value, 0, 4095, -255, 255);
  myData.joy2_y = map(joy2_y_value, 0, 4095, -255, 255);
  myData.joy2_z = joy2_z_value;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(toyAddress, (uint8_t *)&myData, sizeof(myData));

  if (result == ESP_OK) {
    Serial.println("Sending confirmed");
  } else {
    Serial.println("Sending error");
  }

  // Write the values to serial monitor for debugging
  Serial.print("M" + String(myData.joy1_x) + "," + String(myData.joy1_y) + "\n"); // Movement values
  Serial.print("C" + String(myData.joy2_x) + "," + String(myData.joy2_y) + "," + String(myData.joy2_z) + "\n"); // Claw values

  delay(500);
}
