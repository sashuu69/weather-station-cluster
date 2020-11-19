/*
 * Project Name: Home Weather Station Mini
 * Program Name: NodeMCU code
 * Created on: 20/11/2020 02:11:00 AM
 * Last Modified: 20/11/2020 02:11:00 AM
 * Created by: Sashwat K
 */

#include "DHT.h"
#define DHTPIN 5
#define DHTTYPE DHT22

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP280 bmp; // I2C

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();

  if (!bmp.begin()) {
    Serial.println("BMP280 Sensor Error");
    while (1);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

void loop() {
  // put your main code here, to run repeatedly:
  float dht_humidity = dht.readHumidity();
  float dht_temperature = dht.readTemperature();
  float dht_heat_index = dht.computeHeatIndex(dht_temperature, dht_humidity, false);

  float bmp_temperature = bmp.readTemperature();
  float bmp_pressure = bmp.readPressure();
  float bmp_altitude = bmp.readAltitude();

  Serial.println("DHT22 Sensor");
  Serial.println("============");
  Serial.print("Humidity: ");Serial.println(dht_humidity);
  Serial.print("Temperature: ");Serial.println(dht_temperature);
  Serial.print("dht_heat_index");Serial.println(dht_heat_index);
  
  Serial.println("BMP280 Sensor");
  Serial.println("============+");
  Serial.print("Temperature: ");Serial.println(bmp_temperature);
  Serial.print("Pressure: ");Serial.println(bmp_pressure);
  Serial.print("Altitude: ");Serial.println(bmp_altitude);

  delay(2000);
}
