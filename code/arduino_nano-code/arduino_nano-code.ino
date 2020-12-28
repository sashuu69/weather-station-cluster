/*
 * Project Name: Home Weather Station Mini
 * Program Name: Arduino Nano code
 * Created on: 13/12/2020 08:04:00 PM
 * Last Modified: 28/12/2020 09:15:00 PM
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
  Serial.begin(9600);
  Serial.println("Initialising sensors...");
  
  // Initialise DHT22
  dht.begin();
  
  // Initialise BMP280
  int bmp_lib = bmp.begin();
  if(!bmp_lib){
    Serial.println("\t BMP init failed!");
    Serial.flush();
    abort();
  }
  else {
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                    Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                    Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                    Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                    Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
  }

  // Initialise RTC module
  if (! rtc.begin()) {
    Serial.println("\t Couldn't find RTC");
    Serial.flush();
    abort();
  }
  if (! rtc.isrunning()) {
    Serial.println("\t RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Initialise rain sensor
  pinMode(INPUT, RAINSENSORPIN);
  
  // Initialise rain guage
  pinMode(INPUT, RAINGUAGEPIN);

  // Begin software Serial
  s_serial_to_esp.begin(4800);
  
  Serial.println("initialization complete...\n");
}

void loop() {
  // put your main code here, to run repeatedly:

  // Read data from DHT22
  float dht_humidity = dht.readHumidity();
  float dht_temperature = dht.readTemperature();
  float dht_heat_index = dht.computeHeatIndex(dht_temperature, dht_humidity, false);

  // Read data from BMP280
  double bmp_temperature, bmp_pressure, bmp_altitude;
  bmp_temperature = bmp.readTemperature();
  /* bmp_pressure = pressure_value / 100 + const
   * const = 1018.33 - (get pressure value of your location from https://en.allmetsat.com/metar-taf/)
   */
  bmp_pressure = bmp.readPressure() / 100.00 + 5.33;
  bmp_altitude = bmp.readAltitude(bmp_pressure);

  // Read data from RTC module
  DateTime now = rtc.now();
  String rtc_date = now.toString("DD-MM-YYYY");
  String rtc_time = now.toString("hh:mm:ss");

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

  // Send JSON via Serial
  serializeJson(doc, s_serial_to_esp);
  Serial.println("Loop complete..");

  delay(1000);
}
