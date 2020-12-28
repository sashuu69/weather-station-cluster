/*
 * Project Name: Home Weather Station Mini
 * Program Name: Debug code (NodeMCU)
 * Created on: 03/12/2020 11:04:00 PM
 * Last Modified: 27/12/2020 09:58:00 PM
 * Created by: Sashwat K
 */

 
#include <SPI.h>
#include <SD.h>

File root;

// LED Pins
#define LED1 D0

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

  // Initialise Micro SD Card
  Serial.println("1. Initialising SD card...");
  if (!SD.begin(4)) {
    Serial.println("\t Initialisation failed!!!");
    while (1);
  }
  
  Serial.println("2. Initialising LED 1...");
  pinMode(LED1, OUTPUT);

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

  // Testing LED2
  Serial.println("2. Testing LED1..");
  digitalWrite(LED1, HIGH);
  delay(1000);
  digitalWrite(LED1, LOW);
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
