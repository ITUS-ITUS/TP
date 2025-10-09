#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

// WiFi credentials
const char* ssid = "Redmi Note 11T 5G";     //  WiFi SSID
const char* password = "123456789";  // WiFi Password

ESP8266WiFiMulti WiFiMulti;

void setup() {
  pinMode(D2, OUTPUT);      // LED/Buzzer connected to D2
  Serial.begin(9600);

  // Connect to WiFi
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);

  Serial.println("Connecting to WiFi...");
  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
void loop() {
  if (WiFiMulti.run() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;
    // PC's LAN IP
        String url = "http://10.124.223.6/IoT_Practicals/fetch_get.php?name=LEDON";  
    //String url = "http://172.20.10.5/IoT_Practicals/fetch_get.php?name=LEDON";  
    Serial.println("[HTTP] Requesting: " + url);
    http.begin(client, url);  // Start connection
    int httpCode = http.GET();  // Send GET request

    if (httpCode > 0) {
      Serial.printf("[HTTP] Response code: %d\n", httpCode);

      if (httpCode == 200) {
        String payload = http.getString();
        Serial.println("Server Response: " + payload);
        if (payload == "ON") {
          digitalWrite(D2, HIGH);
          Serial.println("LED/Buzzer ON");
          delay(2000);
          digitalWrite(D2,LOW);
        } else if (payload == "OFF") {
          digitalWrite(D2,LOW);
          Serial.println("LED/Buzzer OFF");
        }
      }
    } else {
      Serial.printf("[HTTP] GET failed, error: %s\n",
                    http.errorToString(httpCode).c_str());
    }
    http.end();  // Close connection
  }
  delay(2000);  // Wait 2 seconds before next request
}
