#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include "ThingSpeak.h"

// --- WiFi credentials ---
const char* ssid = "OnePlus Nord";
const char* password = "12345679";

// --- ThingSpeak credentials ---
unsigned long channelID = 3107866;  
const char* writeAPIKey = "8NNSLGDP0BCI7W0Q";

// --- SoftwareSerial pins ---
#define RX_PIN 13  // D7 on NodeMCU (connect to Arduino TX)
#define TX_PIN 15  // D8 (optional, only if sending data back to Arduino)

SoftwareSerial mySerial(RX_PIN, TX_PIN);  // RX, TX
WiFiClient client;  // WiFi client for ThingSpeak

void setup() {
  Serial.begin(115200);   // Debug serial
  mySerial.begin(9600);   // Communication with Arduino

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  // Wait until connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize ThingSpeak
  ThingSpeak.begin(client);

  Serial.println("ESP8266 ready to receive data from Arduino...");
}

void loop() {
  // Check if data is available from Arduino
  if (mySerial.available()) {
    String distance = mySerial.readStringUntil('\n');
    distance.trim(); // remove any newline or spaces

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

      // ThingSpeak requires at least 15 seconds between updates
      delay(16000);
    }
  }
}
