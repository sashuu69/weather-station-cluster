/*
 * Project Name: Weather Station Cluster
 * Program Name: NodeMCU code
 * Created on: 07/03/2021 09:48:00 AM
 * Last Modified: 07/03/2021 09:48:00 AM
 * Created by: Sashwat K
 */
 
// WiFi access point manager
#include <WiFiManager.h> 
WiFiManager wifiManager;

// For OTA updates
#include <ArduinoOTA.h>

// Micro SD card Module
#include <SPI.h>
#include <SD.h>
File sd_card;
#define CS_PIN  15

// JSON connection
#include "ArduinoJson.h"

// Software Serial
#include <SoftwareSerial.h> // Library for Software Serial
SoftwareSerial s_serial_to_mega(2,0); //RX, TX

// LED - D4 - GPIO2
#define SDCRDERRLEDPIN 2

int counter = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("-------------------------");
  Serial.println(" Weather Station Cluster ");
  Serial.println("-------------------------");
  Serial.println("Sensor Initialisation \n");

  // Initialise LED
  pinMode(SDCRDERRLEDPIN, OUTPUT);

  // Initialise LED to off
  digitalWrite(SDCRDERRLEDPIN, LOW);

  Serial.print("1. Initialising SD card module");
  if (!SD.begin(CS_PIN)) {
    digitalWrite(SDCRDERRLEDPIN, HIGH);
    Serial.println("\t Failed");
    while (1);
  }
  else {
    Serial.println("\t Success");
  }

  Serial.print("2. OTA updater initialised");

  wifiManager.autoConnect("Weather Station Cluster");

  ArduinoOTA.setPort(8266);
  ArduinoOTA.setHostname("Weather Station Cluster");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("\t Success");
  
  s_serial_to_mega.begin(4800);
  Serial.println("Initialisation complete");
  Serial.println("--------------------------------");

  Serial.println("Fetch data from SDcard");
  if (SD.exists("hostname.txt")) {
    Serial.println("hostname.txt folder already exists.");
  }
  else {
    Serial.println("hostname.txt folder doesn't exist");
    Serial.flush();
    abort();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (s_serial_to_mega.available()) {
    DynamicJsonDocument doc(1024);
    DeserializationError err = deserializeJson(doc, s_serial_to_mega);
    if (err == DeserializationError::Ok) {
      float dht_humidity = doc["dht_humidity"];
      float dht_temperature = doc["dht_temperature"];
      float dht_heat_index = doc["dht_heat_index"];
      double bmp_temperature = doc["bmp_temperature"];
      double bmp_pressure = doc["bmp_pressure"];
      double bmp_altitude = doc["bmp_altitude"];
      String rtc_day = doc["rtc_day"];
      String rtc_month = doc["rtc_month"];
      String rtc_year = doc["rtc_year"];
      String rtc_hour = doc["rtc_hour"];
      String rtc_minutes = doc["rtc_minutes"];
      String rtc_seconds = doc["rtc_seconds"];
      int rain_sensor_data_analog = doc["rain_sensor_data_analog"];
      int rain_sensor_data_digital = doc["rain_sensor_data_digital"];
      int rain_guage_data = doc["rain_guage_data"];

      if (WiFi.status() == WL_CONNECTED) {
        while(WiFi.status() == WL_CONNECTED){
          ArduinoOTA.handle();
        }
      }

      Serial.println("--------------------------------------------");
      Serial.print("1. Humidity (DHT22): ");Serial.println(dht_humidity);
      Serial.print("2. Temperature (DHT22): ");Serial.println(dht_temperature);
      Serial.print("3. Heat Index (DHT22): ");Serial.println(dht_heat_index);
      Serial.print("4. Temperature (BMP280): ");Serial.println(bmp_temperature);
      Serial.print("5. Pressure (BMP280): ");Serial.println(bmp_pressure);
      Serial.print("6. Altitude (BMP280): ");Serial.println(bmp_altitude);
      Serial.print("7. Day (RTC): ");Serial.println(rtc_day);
      Serial.print("8. Month (RTC): ");Serial.println(rtc_month);
      Serial.print("9. Year (RTC): ");Serial.println(rtc_year);
      Serial.print("10. Hour (RTC): ");Serial.println(rtc_hour);
      Serial.print("11. Minutes (RTC): ");Serial.println(rtc_minutes);
      Serial.print("12. Seconds (RTC): ");Serial.println(rtc_seconds);
      Serial.print("13. Rain Sensor (Analog): ");Serial.println(rain_sensor_data_analog);
      Serial.print("14. Rain Sesnor (Digital): ");Serial.println(rain_sensor_data_digital);
      Serial.print("15. Rain Guage: ");Serial.println(rain_guage_data);

      if(counter%30 == 0) {
        Serial.println("Code to send data");
        
      }
      counter++;
    }
    else {
    // Print error to the "debug" serial port
    Serial.print("deserializeJson() returned ");
    Serial.println(err.c_str());

    // Flush all bytes in the "link" serial port buffer
    while (s_serial_to_mega.available() > 0)
      s_serial_to_mega.read();
    }
  }
}
