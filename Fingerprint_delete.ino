#include <Arduino.h>
#include <Adafruit_Fingerprint.h>

#define FINGERPRINT_TX 25 // TX pin
#define FINGERPRINT_RX 26 // RX pin

// Initialize UART2 with the defined pins
HardwareSerial serialPort(2); // Use UART2
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&serialPort);

void setup() {
  Serial.begin(9600);
  serialPort.begin(57600, SERIAL_8N1, FINGERPRINT_RX, FINGERPRINT_TX); // Initialize UART2 with TX and RX pins

  // Initialize fingerprint sensor
  finger.begin(57600);

  // Verify the password with the sensor
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1);
  }

  // Call function to delete all fingerprints
  Serial.println("Attempting to delete all fingerprints...");
  deleteAllFingerprints();
}

void loop() {
  // Add your loop logic here if needed (e.g., handling alcohol sensor or other tasks)
}

// Function to delete all fingerprints
void deleteAllFingerprints() {
  uint8_t result = finger.emptyDatabase();

  // Check the result of the operation
  if (result == FINGERPRINT_OK) {
    Serial.println("All fingerprints have been successfully deleted!");
  } else if (result == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Error: Could not communicate with the sensor.");
  } else if (result == FINGERPRINT_BADPACKET) {
    Serial.println("Error: Received a bad packet from the sensor.");
  } else {
    Serial.println("Error: Unknown issue when attempting to delete fingerprints.");
  }
}
