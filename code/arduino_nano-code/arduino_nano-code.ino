/*
 * Project Name: Home Weather Station Mini
 * Program Name: Arduino Nano code
 * Created on: 13/12/2020 08:04:00 PM
 * Last Modified: 13/12/2020 11:47:00 PM
 * Created by: Sashwat K
 */

// JSON connection
#include "ArduinoJson.h"

// I2C connection
#define SLAVE_ADDRESS 0x05

// DHT22
#include "DHT.h"
#define DHTPIN 3
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
float dht_humidity, dht_temperature, dht_heat_index;

// BMP280
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
Adafruit_BMP280 bmp;
double bmp_temperature, bmp_pressure, bmp_altitude;

// Rain Sensor
#define RAINSENSORPIN A3
int rain_sensor_data;

// Rain Guage
#define RAINGUAGEPIN 4
int rain_guage_data;

// Connection LED
#define CONNSTATUSLEDPIN 5
bool led_flag;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Initialising sensors...");
  
  // Initialise DHT22
  dht.begin();
  
  // Initialise BMP280
  bmp.begin();
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  // Initialise rain sensor
  pinMode(INPUT, RAINSENSORPIN);

  // Initialise rain guage
  pinMode(INPUT, RAINGUAGEPIN);

  // Initialise I2C connection LED
  pinMode(OUTPUT, CONNSTATUSLEDPIN);

  // Initialise I2C connection
  Wire.begin(SLAVE_ADDRESS);
  Wire.onRequest(sendValue);
  Wire.onReceive(receiveEvent);
  
  Serial.println("initialization complete.");
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // Read data from DHT22
  dht_humidity = dht.readHumidity();
  dht_temperature = dht.readTemperature();
  dht_heat_index = dht.computeHeatIndex(dht_temperature, dht_humidity, false);

  // Read data from BMP280
  bmp_temperature = bmp.readTemperature();
  bmp_pressure = bmp.readPressure();
  bmp_altitude = bmp.readAltitude(1013.25);

  // Read data from Rain Sensor
  rain_sensor_data = analogRead(RAINSENSORPIN);

  // Read data from Rain Guage
  rain_guage_data = digitalRead(RAINGUAGEPIN);

  Serial.println("1. DHT22 Data:-");
  Serial.print("\t i. Humidity: "); Serial.println(dht_humidity);
  Serial.print("\t ii. Temperature: "); Serial.println(dht_temperature);
  Serial.print("\t iii. Heat Index: "); Serial.println(dht_heat_index);

  Serial.println("2. BMP280 Data:-");
  Serial.print("\t i. Pressure: "); Serial.println(bmp_pressure);
  Serial.print("\t ii. Altitude: "); Serial.println(bmp_altitude);
  Serial.print("\t iii. Temperature: "); Serial.println(bmp_temperature);

  Serial.println("3. Rain Sensor Data:-");
  Serial.print("\t i. Value: ");Serial.println(rain_sensor_data);

  Serial.println("4. Rain Guage Data:-");
  Serial.print("\t i. Value: ");Serial.println(rain_guage_data);

  delay(1000);
}

void sendValue() {
  DynamicJsonDocument doc(200);
  doc["dht_humidity"] = dht_humidity;
  doc["dht_temperature"] = dht_temperature;
  doc["dht_heat_index"] = dht_heat_index;
  
  doc["bmp_pressure"] = bmp_pressure;
  doc["bmp_altitude"] = bmp_altitude;
  doc["bmp_temperature"] = bmp_temperature;

  doc["rain_sensor_data"] = rain_sensor_data;
  doc["rain_guage_data"] = rain_guage_data;
  
  String output;
  serializeJson(doc, output);

  char buffer[200];
  output.toCharArray(buffer, 200);
  Wire.write(buffer);
  Serial.println("Data Sent to NodeMCU via I2C..");
}

void receiveEvent() {
  int last_request = Wire.read();
  Serial.println("Reading data from NodeMCU via I2C..");

  if (last_request == 101) {
    if (led_flag == false) {
      digitalWrite(CONNSTATUSLEDPIN, HIGH);
      led_flag = true;
    }
  }
  else {
    if (led_flag == true) {
      digitalWrite(CONNSTATUSLEDPIN, LOW);
      led_flag = false;
    }
  }
}
