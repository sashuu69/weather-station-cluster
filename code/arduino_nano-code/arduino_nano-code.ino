/*
 * Project Name: Home Weather Station Mini
 * Program Name: Arduino Mega code
 * Board Name : Arduino Mega
 * Created on: 13/12/2020 08:04:00 PM
 * Last Modified: 26/01/2021 09:03:00 PM
 * Created by: Sashwat K
 */

// JSON connection
#include "ArduinoJson.h"

#include <SoftwareSerial.h> // Library for Software Serial
SoftwareSerial s_serial_to_esp(11, 12); //RX, TX

// DHT22
#include "DHT.h"
#define DHTPIN 5
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

// Micro SD card Module
#include <SPI.h>
#include <SD.h>
File sd_card;
int sdcard_chip_select = 53;

// Rain Sensor
#define RAINSENSORPIN A7

// Rain Guage
#define RAINGUAGEPIN 7

// Global counter
int counter = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Home Weather Station Mini");
  Serial.println("-------------------------");
  Serial.println("Sensor Initialisation \n");
  
  // Initialise DHT22
  Serial.print("1. Initialising DHT22");
  dht.begin();
  Serial.println("\t Success");

  // Initialise BMP280
  Serial.print("2. Initialising BMP280");
  int bmp_lib = bmp.begin();
  if(!bmp_lib){
    Serial.println("\t Failed");
    Serial.flush();
    abort();
  }
  else {
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                    Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                    Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                    Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                    Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
    Serial.println("\t Success");
  }

  // Initialise RTC module
  Serial.print("3. Initialising RTC module");
  if (! rtc.begin()) {
    Serial.println("\t Failed");
    Serial.flush();
    abort();
  }
  else {
    Serial.println("\t Success");
  }
  if (! rtc.isrunning()) {
    Serial.println("\t RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  Serial.print("4. Initialising Micro SD card module");
  pinMode(sdcard_chip_select, OUTPUT);
  if (!SD.begin()) {
    Serial.println("\t Failed");
    while (1);
  }
  else {
    Serial.println("\t Success");
  }

  Serial.print("5. Initialising Rain sensor");
  // Initialise rain sensor
  pinMode(INPUT, RAINSENSORPIN);
  Serial.println("\t Success");

  Serial.print("5. Initialising Rain guage");
  // Initialise rain guage
  pinMode(INPUT, RAINGUAGEPIN);
  Serial.println("\t Success");

  // Begin software Serial
  Serial.print("6. Initialising software serial");
  s_serial_to_esp.begin(4800);
  Serial.println("\t Success");
  
  Serial.println("\ninitialization complete");
  Serial.println("-------------------------\n");
}

void loop() {
  // put your main code here, to run repeatedly:

  // Read data from DHT22
  float dht_humidity = dht.readHumidity();
  float dht_temperature = dht.readTemperature();
  float dht_heat_index = dht.computeHeatIndex(dht_temperature, dht_humidity, false);

  // Read data from BMP280
  double bmp_temperature = bmp.readTemperature();
  /* bmp_pressure = pressure_value / 100 + const
   * const = 1018.33 - (get pressure value of your location from https://en.allmetsat.com/metar-taf/)
   */
  double bmp_pressure = bmp.readPressure() / 100.00 + 5.33;
  double bmp_altitude = bmp.readAltitude(bmp_pressure);

  // Read data from RTC module
  DateTime now = rtc.now();
  String rtc_date = now.toString("DD-MM-YYYY");
  String rtc_time = now.toString("hh:mm:ss");

  // Read data from Rain Sensor
  int rain_sensor_data = analogRead(RAINSENSORPIN);

  // Read data from Rain Guage
  int rain_guage_data = digitalRead(RAINGUAGEPIN);

  // Create JSON data
  DynamicJsonDocument doc(512);
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
  doc.clear();

  // Store data in micro sdcard every 30 seconds
  if (counter % 30 == 0) {
    String final_string = rtc_date + "," + rtc_time + "," + dht_humidity + "," + dht_temperature + "," + dht_heat_index + "," + bmp_temperature + "," + bmp_pressure + "," + bmp_altitude + "," + rain_sensor_data + "," + rain_guage_data;
    sd_card = SD.open("sensor_data.txt", FILE_WRITE);
    sd_card.println(final_string);
    sd_card.close();
    counter = 0;
  }
  counter ++;

  // Serial print for debugging
  Serial.println("-----------------------------------");
  Serial.print("RTC Date: "); Serial.println(rtc_date);
  Serial.print("RTC Time: "); Serial.println(rtc_time);
  Serial.print("DHT22 Humidity: "); Serial.print(dht_humidity); Serial.println(" %");
  Serial.print("DHT22 Temperature: "); Serial.print(dht_temperature); Serial.println(" °C");
  Serial.print("BMP280 Temperature: "); Serial.print(bmp_temperature); Serial.println(" °C");
  Serial.print("BMP280 Pressure: "); Serial.print(bmp_pressure); Serial.println(" mbar");
  Serial.print("BMP Altitude: "); Serial.print(bmp_altitude); Serial.println(" m");
  Serial.print("Rain Sensor: "); Serial.println(rain_sensor_data);
  Serial.print("Rain Guage: "); Serial.println(rain_guage_data);
  Serial.println("-----------------------------------\n");
  
  delay(1000);
}
