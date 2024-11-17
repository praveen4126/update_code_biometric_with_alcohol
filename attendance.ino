  #include <Arduino.h>
#include <Adafruit_Fingerprint.h>

#define FINGERPRINT_TX 25 // TX pin
#define FINGERPRINT_RX 26 // RX pin
#define ALCOHOL_SENSOR_PIN 33
// Initialize UART2 with the defined pins
HardwareSerial serialPort(2); // use UART2
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&serialPort);

const int ALCOHOL_THRESHOLD = 3500;

void setup() {
  Serial.begin(9600);
  serialPort.begin(57600, SERIAL_8N1, FINGERPRINT_RX, FINGERPRINT_TX); // Initialize UART2 with TX and RX pins

  finger.begin(57600); // Initialize fingerprint sensor

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1);
  }
}

void loop() {
  int fingerprintID = getFingerprintID(); // Check for a fingerprint
  if (fingerprintID >= 0) 
  {
    delay(5000);
    int alcoholLevel = analogRead(ALCOHOL_SENSOR_PIN); // Read alcohol level from sensor
    delay(100); // Small delay to allow sensor reading stabilization

    // Send ID and alcohol level data to the computer
    Serial.print("ID:");
    Serial.print(fingerprintID);
    Serial.print(",ALC:");
    Serial.println(alcoholLevel);

    // Check alcohol level and print status based on threshold
    if (alcoholLevel >= ALCOHOL_THRESHOLD) {
      Serial.println("Status: Absent, NG"); // Alcohol level above threshold
    } else {
      Serial.println("Status: Present, OK"); // Alcohol level below threshold
    }

    delay(2000); // Delay before the next check
  }
}

// Function to get fingerprint ID from the sensor
int getFingerprintID() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) return -1;

  // Return found fingerprint ID number
  Serial.print("Fingerprint ID found: ");
  Serial.println(finger.fingerID);
  return finger.fingerID;
}
