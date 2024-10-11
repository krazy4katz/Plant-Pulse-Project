#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>

// DHT sensor settings
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE); 

// Wi-Fi settings
const char* ssid = "your-SSID";  // Replace with your network SSID
const char* password = "your-PASSWORD";  // Replace with your network password

// Flask server address
const char* serverName = "http://<FLASK_SERVER_IP>:5000/sensor-data";  // Replace with Flask server IP

void setup() {
  Serial.begin(115200);

  // Initialize DHT sensor
  dht.begin();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" Connected to Wi-Fi");
}

void loop() {
  if(WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Start HTTP connection with the server
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    // Collect sensor data
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    
    // Check if data is valid
    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    // Prepare the JSON data to send
    String jsonData = "{";
    jsonData += "\"Temperature\":" + String(temperature) + ",";
    jsonData += "\"Humidity\":" + String(humidity) + ",";
    jsonData += "\"PH\":6.5,";  // Assuming some dummy value for PH
    jsonData += "\"Nitrogen\":150,";  // Dummy value
    jsonData += "\"Phosphorus\":120,";  // Dummy value
    jsonData += "\"Potassium\":100,";  // Dummy value
    jsonData += "\"Overall_Health\":\"Healthy\"";
    jsonData += "}";

    // Send the POST request
    int httpResponseCode = http.POST(jsonData);
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Server response: " + response);
    } else {
      Serial.println("Error sending POST request: " + String(httpResponseCode));
    }

    // End the HTTP connection
    http.end();
  }
  delay(60000);  // Send data every 60 seconds
}
