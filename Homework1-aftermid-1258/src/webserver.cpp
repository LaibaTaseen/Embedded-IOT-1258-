#include <WiFi.h>
#include <WebServer.h>
#include "DHT.h"

// ---------- Wi-Fi credentials ----------
const char* ssid = "Wirelessnet225";      // replace with your Wi-Fi name
const char* pass = "aa11bb22cc33";  // replace with your Wi-Fi password

// ---------- DHT sensor ----------
#define DHTPIN 23
#define DHTTYPE DHT11   // or DHT22
DHT dht(DHTPIN, DHTTYPE);

// ---------- Webserver ----------
WebServer server(80);

// ---------- HTML page ----------
String htmlPage(float temperature, float humidity) {
  String html = "<!DOCTYPE html><html><head><title>ESP32 Webserver</title>";
  html += "<meta http-equiv='refresh' content='5'>"; // auto refresh
  html += "</head><body>";
  html += "<h2>ESP32 Temperature & Humidity</h2>";
  html += "<p>Temperature: " + String(temperature) + " &deg;C</p>";
  html += "<p>Humidity: " + String(humidity) + " %</p>";
  html += "</body></html>";
  return html;
}

// ---------- Handler for root page ----------
void handleRoot() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (isnan(temp) || isnan(hum)) {
    temp = 0;
    hum = 0;
  }

  server.send(200, "text/html", htmlPage(temp, hum));
}

// ---------- Setup ----------
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting ESP32 Webserver...");

  // Initialize DHT sensor
  dht.begin();

  // Connect to Wi-Fi
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Setup webserver route
  server.on("/", handleRoot);
  server.begin();
  Serial.println("Web server started!");
}

// ---------- Loop ----------
void loop() {
  server.handleClient();  // handle incoming requests
}
