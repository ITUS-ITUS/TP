#include <ESP8266WiFi.h>
#include <DHT.h>

#define DHTPIN D4       // GPIO2 on NodeMCU
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);
// WiFi credentials
const char* ssid = "Redmi Note 11T 5G";     //  WiFi SSID
const char* password = "123456789";  // WiFi Password

const char* serverHost = "10.124.223.6";  // Server IP (no http://)
const int serverPort = 80;               // Default HTTP port

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (isnan(temp) || isnan(hum)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print(" °C, Humidity: ");
  Serial.print(hum);
  Serial.println(" %");

  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    if (client.connect(serverHost, serverPort)) {  // Connect correctly
      String postData = "temperature=" + String(temp) + "&humidity=" + String(hum);

      // Send HTTP POST request
      client.println("POST /IoT_Practicals/store_dht.php HTTP/1.1");
      client.print("Host: ");
      client.println(serverHost);//host:10.124.223.6
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.print("Content-Length: ");
      client.println(postData.length());
      client.println();  // End of headers
      client.print(postData);

      // Wait for server response
      while (client.connected()) {
        String line = client.readStringUntil('\n');
        if (line == "\r") break;  // Headers end
      }

      String response = client.readString();
      Serial.println("Server Response:");
      Serial.println(response);

      client.stop();
    } else {
      Serial.println("Connection to server failed!");
    }
  }

  delay(5000); // Send every 5 seconds
}
