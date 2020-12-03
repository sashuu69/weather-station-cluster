/*
 * Project Name: Home Weather Station Mini
 * Program Name: Debug code
 * Created on: 03/12/2020 11:04:00 PM
 * Last Modified: 03/12/2020 11:04:00 PM
 * Created by: Sashwat K
 */

#include "DHT.h"
#define DHTPIN D5
#define DHTTYPE DHT22

#include "BMP280.h"
#include "Wire.h"
#define P0 1013.25

DHT dht(DHTPIN, DHTTYPE);
BMP280 bmp;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // Initialise DHT22
  Serial.println("Initialising DHT22..");
  dht.begin();

  // Initialise BMP280
  Serial.println("Initialising BMP280..");
  bmp.begin();
  bmp.setOversampling(4);
}

void loop() {
  // put your main code here, to run repeatedly:
    // Read data from DHT22
  float dht_humidity = dht.readHumidity();
  float dht_temperature = dht.readTemperature();
  float dht_heat_index = dht.computeHeatIndex(dht_temperature, dht_humidity, false);
  
  Serial.println("DHT22 Data:-");
  Serial.print("Humidity: "); Serial.println(dht_humidity);
  Serial.print("Temperature: "); Serial.println(dht_temperature);
  Serial.print("Heat Index: "); Serial.println(dht_heat_index);

  double bmp_temperature, bmp_pressure, bmp_altitude;
  char result = bmp.startMeasurment();
  Serial.println("BMP280 Data:-");
  if (result != 0) {
    delay(result);
    result = bmp.getTemperatureAndPressure(bmp_temperature,bmp_pressure);
    if(result!=0) {
      bmp_altitude = bmp.altitude(bmp_pressure,P0);
      Serial.print("Pressure: "); Serial.println(bmp_pressure);
      Serial.print("Altitude: "); Serial.println(bmp_altitude);
      Serial.print("Temperature: "); Serial.println(bmp_temperature);
    }
  }
  delay(5000);
}
