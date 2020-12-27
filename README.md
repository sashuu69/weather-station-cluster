# Home Weather Station Mini

## Introduction

The project is still under progress.

The Home Weather Station Mini is weather station system for ethusiasts that want to measure data for education research purposes.

## Features

1. The device collects the following data:-
    1. Temperature
    2. Humidity
    3. Altitude
    4. Pressure
    5. Rain (in cm)
    6. Rain duration and strength
2. Collects the above mentuioned parameters and stores the data to database.

## Folder Structure

1. code/debug-code: Contains code to debug sensors.
2. code/nodemcu-code: Contains nodemcu code for collecting weather data.
3. docs: PCB images (Top, Bottom and Drill).
4. eagle-design: Eagle design of PCB.

## Library used

1. DHT22 Library - [GitHub](https://github.com/adafruit/DHT-sensor-library)
2. BMP280 Library - [GitHub](https://github.com/adafruit/DHT-sensor-library)
3. RTC Library - [GitHub](https://github.com/adafruit/RTClib)

## CAD Files

1. Rain Guage - [Thingiverse](https://www.thingiverse.com/thing:4434857)
2. Rain sensor support - [Thingiverse](https://www.thingiverse.com/thing:4107951)

## Components Used

1. NodeMCU (x1) - [Tomson Electronics](https://www.tomsonelectronics.com/products/buy-nodemcu-esp8266-esp-12e-with-ch340-online?_pos=4&_sid=e5db98597&_ss=r)
2. Micro SD card module (x1) - [Tomson Electronics](https://www.tomsonelectronics.com/products/micro-sd-card-module)
3. Arduino Nano (x1) - [Tomson Electronics](https://www.tomsonelectronics.com/products/buy-arduino-nano-v3-0-online-india)
4. DHT22 (x1) - [Tomson Electronics](https://www.tomsonelectronics.com/products/dht22-digital-temperature-and-humidity-sensor-module-am2302)
5. BMP 280 (x1) - [Tomson Electronics](https://www.tomsonelectronics.com/products/bmp-280-barometer-precision-atmospheric-pressure-sensor-module)
6. Rain sensor (x1) - [Tomson Electronics](https://www.tomsonelectronics.com/products/rain-drop-detection-sensor-rain-detector-weather-module)
7. Speed Measuring sensor Groove Coupler Module (x1) - [Tomson Electronics](https://www.tomsonelectronics.com/products/speed-measuring-sensor-groove-coupler-module-for-arduino)
8. RTC module (x1) - [Tomson Electronics](https://www.tomsonelectronics.com/products/ds1307-i2c-rtc-ds1307-24c32-real-time-clock-module)
9. Button cell (CR-2032) (x1) - [Tomson Electronics](https://www.tomsonelectronics.com/products/panasonic-cr-2032)
10. Female headers - [Robu (India)](https://robu.in/product/2mm-pitch-female-berg-strip-40x1-2pcs/)
11. Male header pins - [Robu (India)](https://robu.in/product/1x40-berg-strip-male-connector/)
12. DC Barrel Power Jack (x1) - [Tomson Electronics](https://www.tomsonelectronics.com/products/dc-barrel-power-jack)
13. DSN-VC288 Volt/AMp Meter (x1) - [Tomson Electronics](https://www.sunrom.com/p/voltamp-meter-dsn-vc288)
14. Diode IN 4007 (x1) - [Tomson Electronics](https://www.tomsonelectronics.com/products/diode-1n-4007)
15. Capacitor (1uF 63V) (x2) - [Tomson Electronics](https://www.tomsonelectronics.com/products/100uf-16v-radial-electrolytic-capacitor)
16. 7805 5V regulator (x1) - [Amazon (India)](https://www.amazon.in/5-piece-voltage-regulator-7805/dp/0070530572)
17. LED (x3) - [Tomson Electronics](https://www.tomsonelectronics.com/products/led-basic-green-5-mm)
18. Micro SD Card Module (x1) - [Tomson Electronics](https://www.tomsonelectronics.com/products/micro-sd-card-module)
19. Rain drop detection sensor Module (x1) - [Tomson Electronics](https://www.tomsonelectronics.com/products/rain-drop-detection-sensor-rain-detector-weather-module)
20. Resistor (10K Ohms) (x2) - [Tomson Electronics](https://www.tomsonelectronics.com/products/1ko-12ko-carbon-film-resistor)
21. Resistor (220 Ohms) (x3) - [Tomson Electronics](https://www.tomsonelectronics.com/products/100k-ohm-metal-film-resistor?variant=8737726038107)
22. Pole, Wires, tapes and zip ties

Discount code for Tomson Electronics : ZENOOFF2020

## PCB design

### Top View

![Top View](docs/main-top.png)

### Bottom View

![Bottom View](docs/main-bottom.png)

### Drill View

![Drill View](docs/main-drills.png)

## Pre-Installation Steps

1. Connect the sensors as per circuit diagram.
2. Flash the NodeMCU code.
3. Connect to NodeMCU WiFi hotspot (SSID: Home Weather Station Mini) WiFi hotspot.
4. Goto http://192.168.4.1 (NodeMCU gateway).
5. Enter WiFi credentials.
6. After connecting to WiFi, use Fing to get IP address of NodeMCU.
7. Goto http://ip-address and enter username (admin) and password (pasword).
8. Enter server IP address, API key and other necessary details.

## Sensor Info

| Sensor name | Features |
|-------------|----------|
| DHT22 | Temperature, Humidity and Heat Index |
| BMP280 | Temperature, atmospheric pressure and altitude |
| Rain Guage | Rain in CM |
| Rain drop sensor | Rain Duration and Strength |

## Contributions

1. Sashwat K (@sashuu6) <hi@sashwat.in>
2. Tharun P Karun (@tharunpkarun) <tpk@tharun.me>
