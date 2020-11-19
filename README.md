# Home Weather Station Mini

## Introduction

The Home Weather Station Mini is a DIY weather station that collects the following data and uploads the data to firebase:-

1. Temperature
2. Humidity
3. Altitude

## Features

1. Collects temperature, humidity and altitude and stores the data in firebase.
2. A web application to show current weather data.
3. A PyQT application for basic statistics

## Folder Structure

1. application: PyQT application for basic statistic works.
2. docs:
3. eagle-design:
4. nodemcu-code: Contains nodemcu code for collecting weather data, web application to show weather data and upload data to firebase.

## Components Used

1. NodeMCU - [Tomson Electronics](https://www.tomsonelectronics.com/products/buy-nodemcu-esp8266-esp-12e-with-ch340-online?_pos=4&_sid=e5db98597&_ss=r)
2. DHT22 - [Tomson Electronics](https://www.tomsonelectronics.com/products/dht22-digital-temperature-and-humidity-sensor-module-am2302)
3. BMP 280 [Tomson Electronics](https://www.tomsonelectronics.com/products/bmp-280-barometer-precision-atmospheric-pressure-sensor-module)
4. Pole, Wires, tapes and zip ties

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
2. Add the necessary credentials in nodemcu code.
3. Flash the code in NodeMCU

## Sensor Info

| Sensor name | Features |
|-------------|----------|
| DHT22 | Temperature, Humidity and Heat Index |
| BMP280 | Temperature, atmospheric pressure and altitude |

## Contributions

1. Sashwat K (@sashuu6) <sashwat0001@gmail.com>
