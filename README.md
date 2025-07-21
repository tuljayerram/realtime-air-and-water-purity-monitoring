# 🌍 Real-time Air and Water Quality Monitoring System

This is an IoT-based project that monitors *air and water quality in real time* using ESP8266 and various environmental sensors. The data is collected, processed, and visualized using *ThingSpeak* and *Google Apps Script*.

## 🚀 Features

- 📡 Real-time data collection
- 🌫 Air quality detection using MQ135
- 🌡 Temperature and humidity sensing via DHT11
- 💧 Water quality monitoring with TDS sensor
- 📊 Live data visualization on ThingSpeak
- 🔗 Integration with Google Sheets via Google Apps Script

## 🧰 Tech Stack

- *Microcontroller:* ESP8266 (NodeMCU)
- *Sensors:*
  - DHT11 – Temperature & Humidity
  - MQ135 – Air Quality
  - TDS Sensor – Water Quality
- *Platform:* Arduino IDE
- *IoT Dashboard:* ThingSpeak
- *Cloud Integration:* Google Apps Script (for Google Sheets logging)

## 🛠 Circuit Connections

| Sensor     | ESP8266 Pin |
|------------|-------------|
| DHT11      | D4 (GPIO2)  |
| MQ135      | D6 (GPIO12) |
| TDS Sensor | A0          |
| VCC/GND    | 3.3V / GND  |

> *Note:* Use voltage dividers if your TDS sensor outputs more than 3.3V.

## 💡 How It Works

1. The sensors continuously collect air and water quality data.
2. ESP8266 sends the data to ThingSpeak via Wi-Fi.
3. Optionally, data is pushed to Google Sheets using Apps Script.
4. The data is visualized in real-time charts for easy monitoring.
