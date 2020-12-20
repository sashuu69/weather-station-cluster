/*
 * Project Name: Home Weather Station Mini
 * Program Name: Debug code
 * Created on: 03/12/2020 11:04:00 PM
 * Last Modified: 05/12/2020 05:00:00 PM
 * Created by: Sashwat K
 */
 
// RTC module
#include <Wire.h>
#include <RTClib.h>
#include <SPI.h>
#include <SD.h>

RTC_DS1307 rtc;
File root;

// LED Pins
//#define LED1 D8
#define LED2 D4

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("\n=========================");
  Serial.println("Home Weather Station Mini");
  Serial.println("-------- NodeMCU --------");
  Serial.println("------ Debug Mode -------");
  Serial.println("=========================\n");

  Serial.println("*************************");
  Serial.println("Initialising sensors...");



  Serial.println("1. Initialising RTC module..");
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

      // Initialise Micro SD Card
  Serial.println("2. Initialising SD card...");
  if (!SD.begin(4)) {
    Serial.println("\t Initialisation failed!!!");
    while (1);
  }
  
  Serial.println("3. Initialising LED 1...");
//  pinMode(LED1, OUTPUT);

  Serial.println("4. Initialising LED 2...");
  pinMode(LED2, OUTPUT);

  Serial.println("initialisation complete.");
  Serial.println("*************************\n\n\n");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("---------------------------\n");




  // Read data from Micro SD Card Module
  Serial.println("1. Testing Micro SD Card Module..");
  root = SD.open("/");
  printDirectory(root, 0);

  // Read data from RTC module
  Serial.println("2. Testing RTC module..");
  Wire.begin(D1,D2);
  DateTime now = rtc.now();

  char buf1[] = "hh:mm";
  Serial.println(now.toString(buf1));
  
  char buf2[] = "YYMMDD-hh:mm:ss";
  Serial.println(now.toString(buf2));
  
  char buf3[] = "Today is DDD, MMM DD YYYY";
  Serial.println(now.toString(buf3));
  
  char buf4[] = "MM-DD-YYYY";
  Serial.println(now.toString(buf4));

  // Testing LED2
  Serial.println("4. Testing LED2..");
  digitalWrite(LED2, HIGH);
  delay(1000);
  digitalWrite(LED2, LOW);
  delay(1000);
  
  delay(2000);
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
