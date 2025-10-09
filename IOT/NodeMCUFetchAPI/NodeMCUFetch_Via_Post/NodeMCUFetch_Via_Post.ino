#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

// WiFi credentials
const char* ssid = "RP (2)";     //  WiFi SSID
const char* password = "123456789";  // WiFi Password  

ESP8266WiFiMulti WiFiMulti;

void setup() {
  Serial.begin(9600);
  pinMode(D2, OUTPUT);

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

   
    String url = "http://172.20.10.5/IoT_Practicals/fetch_post.php";  
    // Send POST data
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");  
    String postData = "name=LEDON";   // sending variable `name=LEDON`

    int httpCode = http.POST(postData);  

    if (httpCode > 0) {
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);

      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println("Server Response: " + payload);

        if (payload == "ON") {
          digitalWrite(D2, HIGH);
          Serial.println("LED ON");
        } else if (payload == "0") {
          digitalWrite(D2, LOW);
          Serial.println("LED OFF");
        }
      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

  delay(2000);  // send every 2 sec
}
