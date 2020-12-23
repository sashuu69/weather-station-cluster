/*
 * Project Name: Home Weather Station Mini
 * Program Name: Arduino Nano code
 * Created on: 13/12/2020 08:04:00 PM
 * Last Modified: 23/12/2020 02:47:00 PM
 * Created by: Sashwat K
 */

// JSON connection
#include "ArduinoJson.h"

#include <SoftwareSerial.h> // Library for Software Serial
SoftwareSerial s_serial_to_esp(6, 7); //RX, TX

// DHT22
#include "DHT.h"
#define DHTPIN 3
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// BMP280
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
Adafruit_BMP280 bmp;

// RTC Module
#include <RTClib.h>
RTC_DS1307 rtc;

// Rain Sensor
#define RAINSENSORPIN A0

// Rain Guage
#define RAINGUAGEPIN 4

void setup() {
  // put your setup code here, to run once:
//  Serial.begin(9600);
//  Serial.println("\n=========================");
//  Serial.println("Home Weather Station Mini");
//  Serial.println("------ Arduino Nano -------");
//  Serial.println("---------------------------");
//  Serial.println("=========================\n");

//  Serial.println("*************************");
//  Serial.println("*************************");
  Serial.println("Initialising sensors...");
  
  // Initialise DHT22
//  Serial.println("1. Initialising DHT22..");
  dht.begin();
  
  // Initialise BMP280
//  Serial.println("2. Initialising BMP280..");
  int bmp_lib = bmp.begin();
  if(!bmp_lib){
    Serial.println("\t BMP init failed!");
    Serial.flush();
    abort();
  }
  else {
//    Serial.println("\t BMP init success!");
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                    Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                    Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                    Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                    Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
  }

  // Initialise RTC module
//  Serial.println("3. Initialising RTC..");
  if (! rtc.begin()) {
    Serial.println("\t Couldn't find RTC");
    Serial.flush();
    abort();
  }
  if (! rtc.isrunning()) {
//    Serial.println("\t RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Initialise rain sensor
//  Serial.println("4. Initialising rain sensor");
  pinMode(INPUT, RAINSENSORPIN);
  
//  Serial.println("5. Initialising rain guage");
  pinMode(INPUT, RAINGUAGEPIN);

  s_serial_to_esp.begin(4800); // Begin software Serial
  
//  Serial.println("initialization complete.");
  Serial.println("*************************");
  Serial.println("*************************\n\n\n");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("---------------------------\n");

  // Read data from DHT22
  float dht_humidity = dht.readHumidity();
  float dht_temperature = dht.readTemperature();
  float dht_heat_index = dht.computeHeatIndex(dht_temperature, dht_humidity, false);

  // Read data from BMP280
  double bmp_temperature, bmp_pressure, bmp_altitude;
  bmp_temperature = bmp.readTemperature();
  bmp_pressure = bmp.readPressure();
  bmp_altitude = bmp.readAltitude(1013.25);

  // Read data from RTC module
  DateTime time = rtc.now();
  String rtc_date = time.timestamp(DateTime::TIMESTAMP_DATE);
  String rtc_time = time.timestamp(DateTime::TIMESTAMP_TIME);

  // Read data from Rain Sensor
  int rain_sensor_data = analogRead(RAINSENSORPIN);

  // Read data from Rain Guage
  int rain_guage_data = digitalRead(RAINGUAGEPIN);

  // Create JSON data
  StaticJsonDocument<100> doc;
  doc["rtc_date"] = rtc_date;
  doc["rtc_time"] = rtc_time;
  doc["dht_humidity"] = dht_humidity;
  doc["dht_temperature"] = dht_temperature;
  doc["dht_heat_index"] = dht_heat_index;
  doc["bmp_temperature"] = bmp_temperature;
  doc["bmp_pressure"] = bmp_pressure;
  doc["bmp_altitude"] = bmp_altitude;
  doc["rain_sensor_data"] = rain_sensor_data;
  doc["rain_guage_data"] = rain_guage_data;
  serializeJson(doc, s_serial_to_esp); // Send JSON via Serial

  // Print info
//  Serial.println("1. DHT22 Data:-");
//  Serial.print("\t i. Humidity: "); Serial.println(dht_humidity);
//  Serial.print("\t ii. Temperature: "); Serial.println(dht_temperature);
//  Serial.print("\t iii. Heat Index: "); Serial.println(dht_heat_index);
//
//  Serial.println("2. BMP280 Data:-");
//  Serial.print("\t i. Pressure: "); Serial.println(bmp_pressure);
//  Serial.print("\t ii. Altitude: "); Serial.println(bmp_altitude);
//  Serial.print("\t iii. Temperature: "); Serial.println(bmp_temperature);
//
//  Serial.println("3. RTC module Data:-");
//  Serial.print("\t i. Timestamp Date: "); Serial.println(rtc_date);
//  Serial.print("\t ii. Timestamp Time: "); Serial.println(rtc_time);
//
//  Serial.println("4. Rain Sensor Data:-");
//  Serial.print("\t i. Value: ");Serial.println(rain_sensor_data);
//
//  Serial.println("5. Rain Guage Data:-");
//  Serial.print("\t i. Value: ");Serial.println(rain_guage_data);

  delay(1000);
}
