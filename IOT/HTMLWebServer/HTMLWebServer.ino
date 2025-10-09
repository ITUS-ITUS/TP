#include <ESP8266WiFi.h>

const char* ssid = "RP (2)";            // Your Wi-Fi SSID
const char* password = "123456789";  // Your Wi-Fi Password

WiFiServer server(80);

const int output7 = 7;
const int output8 = 8;

String header;
String output7State = "off";
String output8State = "off";

void setup() {
  Serial.begin(9600);
  pinMode(output7, OUTPUT);
  pinMode(output8, OUTPUT);
  digitalWrite(output7, LOW);
  digitalWrite(output8, LOW);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.println("IP address: " + WiFi.localIP().toString());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  String currentLine = "";
  header = "";
  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      header += c;
      if (c == '\n') {
        if (currentLine.length() == 0) {
          
          // Handle requests
          if (header.indexOf("GET /7/on") >= 0) {
            output7State = "on";
            digitalWrite(output7, HIGH);
          } else if (header.indexOf("GET /7/off") >= 0) {
            output7State = "off";
            digitalWrite(output7, LOW);
          } else if (header.indexOf("GET /8/on") >= 0) {
            output8State = "on";
            digitalWrite(output8, HIGH);
          } else if (header.indexOf("GET /8/off") >= 0) {
            output8State = "off";
            digitalWrite(output8, LOW);
          }

          // Send HTML page
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html\n");
          client.println("<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
          client.println("<style>.button{padding:16px 40px;font-size:30px;}</style></head><body>");
          client.println("<h1>ESP32 Web Server</h1>");

          client.println("<p>GPIO 7 - State " + output7State + "</p>");
          client.println(output7State=="off" ? "<a href=\"/7/on\"><button class=\"button\">ON</button></a>" :
                                               "<a href=\"/7/off\"><button class=\"button\">OFF</button></a>");

          client.println("<p>GPIO 8 - State " + output8State + "</p>");
          client.println(output8State=="off" ? "<a href=\"/8/on\"><button class=\"button\">ON</button></a>" :
                                               "<a href=\"/8/off\"><button class=\"button\">OFF</button></a>");

          client.println("</body></html>");
          break;
        } else currentLine = "";
      } else if (c != '\r') {
        currentLine += c;
      }
    }
  }
  header = "";
  client.stop();
}
