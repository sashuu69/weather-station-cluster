/*
 * Project Name: Home Weather Station Mini
 * Program Name: Arduino Nano debug code
 * Created on: 13/12/2020 08:04:00 PM
 * Last Modified: 13/12/2020 09:53:00 PM
 * Created by: Sashwat K
 */

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

// Rain Sensor
#define RAINSENSORPIN A0

// Rain Guage
#define RAINGUAGEPIN 4

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("\n=========================");
  Serial.println("Home Weather Station Mini");
  Serial.println("------ Arduino Nano -------");
  Serial.println("------- Debug Mode --------");
  Serial.println("=========================\n");

  Serial.println("*************************");
  Serial.println("*************************");
  Serial.println("Initialising sensors...");
  
  // Initialise DHT22
  Serial.println("1. Initialising DHT22..");
  dht.begin();
  
  // Initialise BMP280
  bmp.begin();
//  Serial.println("2. Initialising BMP280..");
//  if(!bmp.begin()){
//    Serial.println("\t BMP init failed!");
//    while(1);
//  }
//  else {
//    Serial.println("\t BMP init success!");
//    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
//                    Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
//                    Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
//                    Adafruit_BMP280::FILTER_X16,      /* Filtering. */
//                    Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
//  }

  // Initialise rain sensor
  Serial.println("3. Initialising rain sensor");
  pinMode(INPUT, RAINSENSORPIN);
  
  Serial.println("4. Initialising rain guage");
  pinMode(INPUT, RAINGUAGEPIN);
  
  Serial.println("initialization complete.");
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

  Serial.println("1. DHT22 Data:-");
  Serial.print("\t i. Humidity: "); Serial.println(dht_humidity);
  Serial.print("\t ii. Temperature: "); Serial.println(dht_temperature);
  Serial.print("\t iii. Heat Index: "); Serial.println(dht_heat_index);

  // Read data from BMP280
  double bmp_temperature, bmp_pressure, bmp_altitude;
  bmp_temperature = bmp.readTemperature();
  bmp_pressure = bmp.readPressure();
  bmp_altitude = bmp.readAltitude(1013.25);
  
  Serial.println("2. BMP280 Data:-");
  Serial.print("\t i. Pressure: "); Serial.println(bmp_pressure);
  Serial.print("\t ii. Altitude: "); Serial.println(bmp_altitude);
  Serial.print("\t iii. Temperature: "); Serial.println(bmp_temperature);

  // Read data from Rain Sensor
  int rain_sensor_data = analogRead(RAINSENSORPIN);
  Serial.println("3. Rain Sensor Data:-");
  Serial.print("\t i. Value: ");Serial.println(rain_sensor_data);

  // Read data from Rain Guage
  int rain_guage_data = digitalRead(RAINGUAGEPIN);
  Serial.println("4. Rain Guage Data:-");
  Serial.print("\t i. Value: ");Serial.println(rain_guage_data);
  
  Serial.println("---------------------------\n\n");
  
  delay(5000);
}
