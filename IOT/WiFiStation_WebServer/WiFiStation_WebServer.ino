#include <WiFi.h>
#include <WebServer.h>
#include "esp_eap_client.h"   // WPA2-Enterprise API

// ====== University WiFi details ======
const char* ssid = "";        // SSID
const char* pwd = "";      
#define EAP_IDENTITY ""       // Identity
#define EAP_USERNAME ""       // Username
#define EAP_PASSWORD ""  // Password
// =====================================

// Web server on port 80
WebServer server(80);

// Use onboard LED (GPIO 2 for most ESP32 boards)
const int ledPin = 2;

// ====== HTML page with LED controls ======
String htmlPage() {
  String page = "<!DOCTYPE html><html><head><title>ESP32 LED Control</title></head><body>";
  page += "<h1>ESP32 WebServer - LED Control</h1>";
  page += "<p><a href=\"/on\"><button>LED ON</button></a></p>";
  page += "<p><a href=\"/off\"><button>LED OFF</button></a></p>";
  page += "</body></html>";
  return page;
}

void handleRoot() {
  server.send(200, "text/html", htmlPage());
}

void handleLedOn() {
  digitalWrite(ledPin, HIGH);
  server.send(200, "text/html", "<h1>LED is ON</h1><br><a href=\"/\">Back</a>");
}

void handleLedOff() {
  digitalWrite(ledPin, LOW);
  server.send(200, "text/html", "<h1>LED is OFF</h1><br><a href=\"/\">Back</a>");
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);

  // WPA2-Enterprise credentials
  esp_eap_client_set_identity((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY));
  esp_eap_client_set_username((uint8_t *)EAP_USERNAME, strlen(EAP_USERNAME));
  esp_eap_client_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD));

  // Enable WPA2-Enterprise
  esp_wifi_sta_enterprise_enable();

  // Start connection
  WiFi.begin(ssid, pwd);
  Serial.println("Connecting to WPA2-Enterprise WiFi...");

  // Wait until connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Setup server routes
  server.on("/", handleRoot);
  server.on("/off", handleLedOn);
  server.on("/on", handleLedOff);

  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
}
