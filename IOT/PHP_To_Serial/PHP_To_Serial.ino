#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

// WiFi credentials
const char* ssid = "RP (2)";
const char* password = "123456789";

// PHP endpoint that returns JSON
const char* serverName = "http://172.20.10.5/IoT_Practicals/fetch_dht.php";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    http.begin(client, serverName);  // Connect to server
    int httpCode = http.GET();       // Send GET request

    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();  // Get response as String
        Serial.println("Raw JSON:");
        Serial.println(payload);

        // Allocate JSON buffer (use capacity based on expected data size)
        StaticJsonDocument<1024> doc;

        // Parse JSON
        DeserializationError error = deserializeJson(doc, payload);
        if (error) {
          Serial.print("JSON parsing failed: ");
          Serial.println(error.c_str());
        } else {
          // JSON is an array of objects
          for (JsonObject obj : doc.as<JsonArray>()) {
            float temp = obj["temperature"];
            float hum  = obj["humidity"];
            const char* ts = obj["timestamp"];

            Serial.print("Temperature: ");
            Serial.print(temp);
            Serial.print(" °C, Humidity: ");
            Serial.print(hum);
            Serial.print(" %, Time: ");
            Serial.println(ts);
          }
        }
      }
    } else {
      Serial.print("HTTP GET failed, error: ");
      Serial.println(http.errorToString(httpCode));
    }

    http.end(); // Free resources
  }

  delay(10000); // Fetch every 10 seconds
}
