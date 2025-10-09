#include <WiFi.h>
#include "ThingSpeak.h"

// --- WiFi credentials ---
const char* ssid = "OnePlus Nord";
const char* password = "12345679";

// --- ThingSpeak credentials ---
unsigned long channelID = 3107866;
const char* writeAPIKey = "8NNSLGDP0BCI7W0Q";

// --- ESP32 Hardware Serial2 pins ---
#define RXD2 16   // GPIO16 (connect to Arduino TX)
#define TXD2 17   // GPIO17 (optional, if sending data back to Arduino)

WiFiClient client;

void setup() {
  Serial.begin(115200);   // For debug messages
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);  // For Arduino communication

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize ThingSpeak
  ThingSpeak.begin(client);

  Serial.println("ESP32 ready to receive data from Arduino...");
}

void loop() {
  if (Serial2.available()) {
    String distance = Serial2.readStringUntil('\n');
    distance.trim();  // Clean up whitespace/newlines

    if (distance.length() > 0) {
      Serial.print("Received from Arduino: ");
      Serial.println(distance);

      // Upload to ThingSpeak
      ThingSpeak.setField(1, distance);
      int statusCode = ThingSpeak.writeFields(channelID, writeAPIKey);

      if (statusCode == 200) {
        Serial.println("Update successful!");
      } else {
        Serial.print("Problem uploading data. HTTP error code: ");
        Serial.println(statusCode);
      }

      delay(16000); // ThingSpeak requires ≥15 s between updates
    }
  }
}
