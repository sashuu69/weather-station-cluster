/*
 * Project Name: Home Weather Station Mini
 * Program Name: Debug code (NodeMCU)
 * Created on: 03/12/2020 11:04:00 PM
 * Last Modified: 27/12/2020 09:58:00 PM
 * Created by: Sashwat K
 */

#include <ArduinoJson.h> // Library for JSON
#include <SoftwareSerial.h> // Library for Software Serial
SoftwareSerial s_serial_to_nano(D4, D3); // Software Serial with Nano (RX,TX)

// LED Pins
#define LED1 D0

String rtc_date;
String rtc_time;
float dht_humidity;
float dht_temperature;
float dht_heat_index;
double bmp_temperature;
double bmp_pressure;
double bmp_altitude;
int rain_sensor_data;
int rain_guage_data;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("\n=========================");
  Serial.println("Home Weather Station Mini");
  Serial.println("-------- NodeMCU --------");
  Serial.println("------ Debug Mode -------");
  Serial.println("=========================\n");

  Serial.println("*************************");
  Serial.println("Initialising sensors...");

  Serial.println("1. Initialising software serial...");
  s_serial_to_nano.begin(4800);
  
  Serial.println("2. Initialising LED 1...");
  pinMode(LED1, OUTPUT);

  Serial.println("initialisation complete.");
  Serial.println("*************************\n\n\n");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (s_serial_to_nano.available()) {
    Serial.println("---------------------------\n");
    digitalWrite(LED1, HIGH);
    
    DynamicJsonDocument doc(512);
    deserializeJson(doc, s_serial_to_nano);
    rtc_date = doc["rtc_date"].as<String>();
    rtc_time = doc["rtc_time"].as<String>();
    dht_humidity = doc["dht_humidity"].as<float>();
    dht_temperature = doc["dht_temperature"].as<float>();
    dht_heat_index = doc["dht_heat_index"].as<float>();
    bmp_temperature = doc["bmp_temperature"].as<double>();
    bmp_pressure = doc["bmp_pressure"].as<double>();
    bmp_altitude = doc["bmp_altitude"].as<double>();
    rain_sensor_data = doc["rain_sensor_data"].as<int>();
    rain_guage_data = doc["rain_guage_data"].as<int>();

    Serial.println("1. Values from Arduino Nano (Software Serial):-");
    Serial.print("\t RTC Date: "); Serial.println(rtc_date);
    Serial.print("\t RTC Time: "); Serial.println(rtc_time);
    Serial.print("\t DHT22 Humidity: "); Serial.println(dht_humidity);
    Serial.print("\t DHT22 Temperature: "); Serial.println(dht_temperature);
    Serial.print("\t DHT22 Heat Index: "); Serial.println(dht_heat_index);
    Serial.print("\t BMP280 Temperature: "); Serial.println(bmp_temperature);
    Serial.print("\t BMP280 Pressure: "); Serial.println(bmp_pressure);
    Serial.print("\t BMP280 Altitude: "); Serial.println(bmp_altitude);
    Serial.print("\t Rain Sensor: "); Serial.println(rain_sensor_data);
    Serial.print("\t Rain Guage: "); Serial.println(rain_guage_data);

    Serial.println("---------------------------\n");
  }
}
