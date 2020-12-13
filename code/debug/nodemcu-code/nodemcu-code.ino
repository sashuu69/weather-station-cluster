/*
 * Project Name: Home Weather Station Mini
 * Program Name: Debug code
 * Created on: 03/12/2020 11:04:00 PM
 * Last Modified: 05/12/2020 05:00:00 PM
 * Created by: Sashwat K
 */

// DHT22
#include "DHT.h"
#define DHTPIN D4
#define DHTTYPE DHT22

// BMP280
#include "BMP280.h"
#include "Wire.h"
#define P0 1013.25

// Micro SD card Module
#include <SPI.h>
#include <SD.h>

DHT dht(DHTPIN, DHTTYPE);
BMP280 bmp;
File root;

int rain_sensor = A0;

int led1 = D0;
int led2 = D3;

void setup() {
  // put your setup code here, to run once:
  delay(2000);
  Serial.begin(9600);
  Serial.println("\n=========================");
  Serial.println("Home Weather Station Mini");
  Serial.println("------ Debug Mode -------");
  Serial.println("=========================\n");

  Serial.println("*************************");
  Serial.println("Initialising sensors...");
  // Initialise DHT22
  Serial.println("1. Initialising DHT22..");
  dht.begin();

  // Initialise BMP280
  Serial.println("2. Initialising BMP280..");
  if(!bmp.begin()){
    Serial.println("\t BMP init failed!");
    while(1);
  }
  else Serial.println("\t BMP init success!");
  bmp.setOversampling(4);

  // Initialise Micro SD Card
  Serial.println("3. Initializing SD card...");
  if (!SD.begin(4)) {
    Serial.println("\t initialization failed!!!");
    while (1);
  }
  
  Serial.println("4. Initializing LED 1...");
  pinMode(led1, OUTPUT);

  Serial.println("5. Initializing LED 2...");
  pinMode(led2, OUTPUT);

  Serial.println("initialization complete.");
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
  char result = bmp.startMeasurment();
  Serial.println(result);
  Serial.println("2. BMP280 Data:-");
  if (result != 0) {
    delay(result);
    result = bmp.getTemperatureAndPressure(bmp_temperature,bmp_pressure);
    if(result!=0) {
      bmp_altitude = bmp.altitude(bmp_pressure,P0);
      Serial.print("\t i. Pressure: "); Serial.println(bmp_pressure);
      Serial.print("\t ii. Altitude: "); Serial.println(bmp_altitude);
      Serial.print("\t iii. Temperature: "); Serial.println(bmp_temperature);
    }
  }

  // Read data from Micro SD Card Module
  root = SD.open("/");
  printDirectory(root, 0);

  // Read data from Rain Sensor
  Serial.println("3. Rain sensor:-");
  int sensorValue = analogRead(rain_sensor);
  int outputValue = map(sensorValue, 0, 1023, 0, 255);
  Serial.print("\t i. Sensor: "); Serial.print(sensorValue);
  Serial.print("\t ii. Output: "); Serial.println(outputValue);

  Serial.println("4. Testing LED1..");
  digitalWrite(led1, HIGH);
  delay(1000);
  digitalWrite(led1, LOW);
  delay(1000);

  Serial.println("4. Testing LED2..");
  digitalWrite(led2, HIGH);
  delay(1000);
  digitalWrite(led2, LOW);
  delay(1000);
  
  delay(5000);
  Serial.println("---------------------------\n\n");
}

void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}
