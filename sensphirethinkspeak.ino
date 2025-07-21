#include <ESP8266WiFi.h>
#include <DHT.h>
#include <MQ135.h>

// WiFi Credentials
const char* ssid = "VISIONOVA";
const char* password = "12345678";

// ThingSpeak API Key & Server
const char* apiKey = "DKIDZR5BCC95318B";
const char* server = "api.thingspeak.com";

// Sensor Pins & Configurations
#define DHTPIN 2          // DHT11 connected to GPIO2 (D4)
#define DHTTYPE DHT11     // DHT11 Sensor
#define TDS_PIN A0        // TDS sensor on analog pin (consider using a multiplexer if needed)
#define MQ135_PIN A0      // MQ135 also uses A0; need a multiplexer or separate pin
#define VREF 3.3          // ESP8266 operates at 3.3V
#define TDS_FACTOR 0.5    // TDS conversion factor

// Sensor Objects
DHT dht(DHTPIN, DHTTYPE);
MQ135 gasSensor(MQ135_PIN);
WiFiClient client;

// WiFi Connection Function
void connectWiFi() {
    Serial.print("Connecting to WiFi: ");
    Serial.println(ssid);
    
    WiFi.begin(ssid, password);
    unsigned long startAttemptTime = millis();

    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
        delay(1000);
        Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConnected to WiFi!");
    } else {
        Serial.println("\nWiFi connection failed!");
    }
}

void setup() {
    Serial.begin(115200);
    dht.begin();
    connectWiFi();
}

void loop() {
    // Read Sensor Data
    float tdsValue = readTDS();
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    float air_quality = readAirQuality(temperature, humidity);

    if (isnan(temperature) || isnan(humidity)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    // Print Sensor Values in Order
    Serial.print("TDS (ppm): ");
    Serial.print(tdsValue);
    Serial.print("\tAir Quality (PPM): ");
    Serial.print(air_quality);
    Serial.print("\tTemperature: ");
    Serial.print(temperature);
    Serial.print("°C\tHumidity: ");
    Serial.print(humidity);
    Serial.println("%");

    // Send Data to ThingSpeak
    sendDataToThingSpeak(tdsValue, air_quality, temperature, humidity);

    delay(20000);  // Wait 20 seconds before sending data again
}

// Function to Read TDS Sensor
float readTDS() {
    int sensorValue = analogRead(TDS_PIN);
    float voltage = sensorValue * (VREF / 1023.0);
    float tds = (voltage * TDS_FACTOR) * 1000;
    return tds;
}

// Function to Read MQ135 Air Quality Sensor
float readAirQuality(float temp, float hum) {
    float ppm = gasSensor.getPPM(); // Uncorrected PPM
    float correctedPPM = gasSensor.getCorrectedPPM(temp, hum);
    return correctedPPM;
}

// Function to Send Data to ThingSpeak
void sendDataToThingSpeak(float tds, float air_quality, float temp, float hum) {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi not connected. Reconnecting...");
        connectWiFi();
    }

    String url = "http://" + String(server) + "/update?api_key=" + apiKey +
                 "&field1=" + String(tds) +
                 "&field2=" + String(air_quality) +
                 "&field3=" + String(temp) +
                 "&field4=" + String(hum);

    if (client.connect(server, 80)) {
        client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                     "Host: " + server + "\r\n" +
                     "Connection: close\r\n\r\n");

        Serial.println("Data sent to ThingSpeak!");
    } else {
        Serial.println("Connection to ThingSpeak failed!");
    }

    client.stop();
}
