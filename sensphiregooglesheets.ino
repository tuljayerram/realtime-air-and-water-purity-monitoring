#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid = "Z90"; 
const char* password = "tulja2006";

// Replace with your Google Apps Script URL
const char* googleScriptURL = "https://script.google.com/macros/s/AKfycbx9TIlal_zw08BJPmjgu5FWfTp7_JOLw8Iv0130YIIWsXAwTwJoQ29R-U1BBfrwuURc/exec";

// Sensor Variables
float tdsValue = 0.0;
float airQuality = 0.0;
float temperature = 0.0;
float humidity = 0.0;

WiFiClientSecure client;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");
  
  client.setInsecure();  // Disable SSL certificate validation
}

void loop() {
  // Replace with actual sensor readings
  tdsValue = readTDS();
  airQuality = readAirQuality();
  temperature = readTemperature();
  humidity = readHumidity();

  sendDataToGoogleSheets(tdsValue, airQuality, temperature, humidity);
  delay(2000);  // Send data every 20 seconds
}

void sendDataToGoogleSheets(float tds, float air, float temp, float hum) {
  if (WiFi.status() == WL_CONNECTED) {
    String url = String(googleScriptURL) + "?tds=" + String(tds) + 
                 "&airQuality=" + String(air) + 
                 "&temperature=" + String(temp) + 
                 "&humidity=" + String(hum);
    
    Serial.println("Sending data to Google Sheets...");
    if (client.connect("script.google.com", 443)) {
      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                   "Host: script.google.com\r\n" +
                   "Connection: close\r\n\r\n");
      Serial.println("Data sent!");
    } else {
      Serial.println("Connection failed!");
    }
    client.stop();
  } else {
    Serial.println("WiFi not connected!");
  }
}

// Dummy sensor reading functions
float readTDS() { return random(100, 500); }
float readAirQuality() { return random(50, 300); }
float readTemperature() { return random(20, 35); }
float readHumidity() { return random(30, 80); }
