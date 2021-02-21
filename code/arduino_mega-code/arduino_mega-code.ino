/*
 * Project Name: Weather Station Cluster
 * Program Name: Arduino Mega code
 * Board Name : Arduino Mega
 * Created on: 13/12/2020 08:04:00 PM
 * Last Modified: 29/01/2021 11:39:00 PM
 * Created by: Sashwat K
 */

// JSON connection
#include "ArduinoJson.h"

// Software Serial
#include <SoftwareSerial.h> // Library for Software Serial
SoftwareSerial s_serial_to_esp(2, 3); //RX, TX

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
#define RAINSENSORANALOGPIN A7
#define RAINSENSORDIGITALPIN  22

// Rain Guage
#define RAINGUAGEPIN 7
int rain_guage_counter = 0;
int rain_guage_flag = 0;
int rain_guage_data = 0;

// LEDs
#define SDREADLEDPIN 29 
#define BMPERRLEDPIN 33
#define RTCERRLEDPIN 35
#define SDCRDERRLEDPIN 31

// Global counter
int counter = 0;
bool sdcard_flag = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("-------------------------");
  Serial.println(" Weather Station Cluster ");
  Serial.println("-------------------------");
  Serial.println("Sensor Initialisation \n");

  // Initialise LEDs
  pinMode(SDREADLEDPIN, OUTPUT);
  pinMode(BMPERRLEDPIN, OUTPUT);
  pinMode(RTCERRLEDPIN, OUTPUT);
  pinMode(SDCRDERRLEDPIN, OUTPUT);

  // Initialise LED to off
  digitalWrite(SDREADLEDPIN, LOW);
  digitalWrite(BMPERRLEDPIN, LOW);
  digitalWrite(RTCERRLEDPIN, LOW);
  digitalWrite(SDCRDERRLEDPIN, LOW);
  
  // Initialise DHT22
  Serial.print("1. Initialising DHT22");
  dht.begin();
  Serial.println("\t Success");

  // Initialise BMP280
  Serial.print("2. Initialising BMP280");
  int bmp_lib = bmp.begin();
  if(!bmp_lib){
    digitalWrite(BMPERRLEDPIN, HIGH);
    Serial.println("\t Failed");
    Serial.flush();
    abort();
  }
  else {
    Serial.println("\t Success");
  }

  // Initialise RTC module
  Serial.print("3. Initialising RTC module");
  if (! rtc.begin()) {
    digitalWrite(RTCERRLEDPIN, HIGH);
    Serial.println("\t Failed");
    Serial.flush();
    abort();
  }
  else {
    Serial.println("\t Success");
  }

  Serial.print("4. Initialising Micro SD card module");
  pinMode(sdcard_chip_select, OUTPUT);
  if (!SD.begin()) {
    digitalWrite(SDCRDERRLEDPIN, HIGH);
    Serial.println("\t Failed");
    while (1);
  }
  else {
    Serial.println("\t Success");
  }

  Serial.print("5. Initialising Rain sensor");
  // Initialise rain sensor
  pinMode(INPUT, RAINSENSORANALOGPIN);
  pinMode(INPUT, RAINSENSORDIGITALPIN);
  Serial.println("\t Success");

  Serial.print("5. Initialising Rain guage");
  // Initialise rain guage
  pinMode(INPUT, RAINGUAGEPIN);
  Serial.println("\t Success");

  // Begin software Serial
  Serial.print("6. Initialising software serial");
  s_serial_to_esp.begin(4800);
  Serial.println("\t Success");
  
  Serial.println("\ninitialiaation complete");
  Serial.println("-------------------------\n");

  Serial.println("Post Initialisation steps");
  Serial.println("-------------------------\n");

  // BMP post initialisation
  Serial.println("Setting BMP settings");
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                    Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                    Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                    Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                    Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  // RTC module
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // For rain guage
  if (SD.exists("rgs.txt")) {
    Serial.println("rain guage status (rgs.txt) file exists");
  }
  else {
    Serial.println("Creating rain guage status (rgs.txt)");
    sd_card = SD.open("rgs.txt", FILE_WRITE);
    sd_card.println(0);
    sd_card.close();
  }
  
  if (SD.exists("rgf.txt")) {
    Serial.println("rain-guage-flag.txt file exists");
  }
  else {
    Serial.println("Creating rain guage flag (rgf.txt");
    sd_card = SD.open("rgf.txt", FILE_WRITE);
    sd_card.println(0);
    sd_card.close();
  }
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
  int rain_sensor_data_analog = analogRead(RAINSENSORANALOGPIN);
  int rain_sensor_data_digital = digitalRead(RAINSENSORDIGITALPIN);

  // Read data from Rain Guage
  sd_card = SD.open("rgs.txt", FILE_READ); // To secure from reset or shutdown
  rain_guage_data = sd_card.read();
  sd_card.close();
  
  sd_card = SD.open("rgf.txt", FILE_READ); // To secure from reset or shutdown
  rain_guage_flag = sd_card.read();
  sd_card.close();
  
  int rain_guage_output = digitalRead(RAINGUAGEPIN);
  if(rain_guage_flag != rain_guage_output) {
    rain_guage_data ++;
    rain_guage_flag = rain_guage_output;
  }

  // Update rain guage flag to file
  // To secure from reset or shutdown
  SD.remove("rgf.txt");
  sd_card = SD.open("rgf.txt", FILE_WRITE);
  sd_card.println(rain_guage_flag);
  sd_card.close();
  
  // Update rain guage count to file
  // To secure from reset or shutdown
  SD.remove("rgs.txt");
  sd_card = SD.open("rgs.txt", FILE_WRITE);
  sd_card.println(rain_guage_data);
  sd_card.close();

  // Reset rain guage every midnight
  if (now.hour() == 0 && now.minute() == 00 && rain_guage_data != 0) {
    rain_guage_data = 0;
  }  

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
  doc["rain_sensor_data_analog"] = rain_sensor_data_analog;
  doc["rain_sensor_data_digital"] = rain_sensor_data_digital;
  doc["rain_guage_data"] = rain_guage_data;
  
  // Send JSON via Serial
  serializeJson(doc, s_serial_to_esp);
  doc.clear();

  // Store data in micro sdcard every 5 seconds
  if (counter % 30 == 0) {
    String final_string = rtc_date + "," + rtc_time + "," + dht_humidity + "," + dht_temperature + "," + dht_heat_index + "," + bmp_temperature + "," + bmp_pressure + "," + bmp_altitude + "," + rain_sensor_data_analog + "," + rain_sensor_data_digital + "," + rain_guage_data;
    sd_card = SD.open("sdt.txt", FILE_WRITE);
    sd_card.println(final_string);
    sd_card.close();
    counter = 0;
    sdcard_flag = true;
  }
  counter ++;

  if (sdcard_flag == true) {
    if (counter == 1) {
      digitalWrite(SDREADLEDPIN, HIGH);
    }
    else if (counter == 2) {
      digitalWrite(SDREADLEDPIN, LOW);
      sdcard_flag = false;
    }
  }

  // Serial print for debugging
  Serial.println("-----------------------------------");
  Serial.print("1. RTC Date: "); Serial.println(rtc_date);
  Serial.print("2. RTC Time: "); Serial.println(rtc_time);
  Serial.print("3. DHT22 Humidity: "); Serial.print(dht_humidity); Serial.println(" %");
  Serial.print("4. DHT22 Temperature: "); Serial.print(dht_temperature); Serial.println(" °C");
  Serial.print("5. DHT22 Heat Index: "); Serial.print(dht_heat_index); Serial.println(" °C");
  Serial.print("6. BMP280 Temperature: "); Serial.print(bmp_temperature); Serial.println(" °C");
  Serial.print("7. BMP280 Pressure: "); Serial.print(bmp_pressure); Serial.println(" mbar");
  Serial.print("8. BMP Altitude: "); Serial.print(bmp_altitude); Serial.println(" m");
  Serial.print("9. Rain Sensor (Analog): "); Serial.println(rain_sensor_data_analog);
  Serial.print("10. Rain Sensor (Digital): "); Serial.println(rain_sensor_data_digital);
  Serial.print("11. Rain Guage: "); Serial.println(rain_guage_data);
  Serial.println("-----------------------------------\n");
  delay(1000);
}
