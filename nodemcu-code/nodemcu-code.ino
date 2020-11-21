/*
 * Project Name: Home Weather Station Mini
 * Program Name: NodeMCU code
 * Created on: 20/11/2020 02:11:00 AM
 * Last Modified: 20/11/2020 02:11:00 AM
 * Created by: Sashwat K
 */
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> 
#define WEB_PORT 80
#include "DHT.h"
#define DHTPIN D5
#define DHTTYPE DHT22

ESP8266WebServer server(WEB_PORT);
DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;

String admin_username = "admin";
String admin_password = "password";
String server_url = "google.com";
String server_api_key = "23R456DUG22458";
String gps_longitude = "0";
String gps_latitude = "0";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();

  WiFiManager wifiManager;
  wifiManager.autoConnect("Home Weather Station Mini");

  server.on("/", handle_OnConnect);
  server.begin();
  
  Serial.println("connected :)");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (WiFi.status() == WL_CONNECTED) {
    while(WiFi.status() == WL_CONNECTED){
      server.handleClient();
      float dht_humidity = dht.readHumidity();
      float dht_temperature = dht.readTemperature();
      float dht_heat_index = dht.computeHeatIndex(dht_temperature, dht_humidity, false);
      
      Serial.println("\nDHT22 Sensor");
      Serial.println("============");
      Serial.print("Humidity: ");Serial.println(dht_humidity);
      Serial.print("Temperature: ");Serial.println(dht_temperature);
      Serial.print("dht_heat_index: ");Serial.println(dht_heat_index);

      delay(5000);
    }
  }
}

void handle_OnConnect() {
  server.send(200, "text/html", SendHTML());
}

String SendHTML() {
  String wifi_ssid = WiFi.SSID().c_str();
  String device_hostname = WiFi.hostname().c_str();
  String device_ip_address = WiFi.localIP().toString();
  String device_subnet = WiFi.subnetMask().toString();
  String device_gateway = WiFi.gatewayIP().toString();
  String device_macaddress = WiFi.macAddress().c_str();
  String server_connection_status;

  if (client.connect(server_url, 80)) {
    server_connection_status = "UP";
  }
  else {
    server_connection_status = "DOWN";
  }
  
  String ptr = "<!DOCTYPE html> <html>\n";
  
  ptr += "<head> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>Home Weather Station Mini</title>\n";
  ptr += "<style>\n";
  ptr += "html {font-family: Helvetica; font-weight: bold; display: inline-block; margin: 0px auto; text-align: center; color: whitesmoke;}\n";
  ptr += "table, th, td {margin-left: auto; margin-right: auto; border: 1px solid whitesmoke;}\n";
  ptr += "body{margin-top: 50px;}\n";
  ptr += "h1 {color: whitesmoke;margin: 50px auto 30px;}\n";
  ptr += "h2 {color: whitesmoke;margin-bottom: 50px;}\n";
  ptr += "button {background-color: #4CAF50; border: none; color: white; text-align: center; text-decoration: none; display: inline-block; margin: 4px 2px; cursor: pointer; border-radius: 4px;}\n";
  ptr += "body {background: linear-gradient(-45deg, #ee7752, #e73c7e, #23a6d5, #23d5ab);background-size: 400% 400%;animation: gradient 15s ease infinite;}\n";
  ptr += "@keyframes gradient {0% {background-position: 0% 50%;}50% {background-position: 100% 50%;}100% {background-position: 0% 50%;}}\n";
  ptr += ".button-off:active {background-color: #2c3e50;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "<script type = \"text/javascript\">\n";
  ptr += "function login() {\n";
  ptr += "var username = prompt(\"Please enter your username\");\n";
  ptr += "var password = prompt(\"Please enter your password\");\n";
  ptr += "if (username == \"" + admin_username +"\" && password == \"" + admin_password + "\") {\n";
  ptr += "alert(\"Login Success\"); }\n";
  ptr += "else {\n";
  ptr += "alert(\"Invalid username and password\");window.location.reload(); } }\n";
  ptr += "login();\n";
  ptr += "</script>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>Home Weather Station Mini Dashboard</h1>\n";
  ptr += "<h2>Device Status</h2>\n";
  ptr += "<table> <tr> <td>WiFi SSID: </td> <td colspan=\"2\">" + wifi_ssid + "</td> </tr> <tr> <td>Hostname: </td> <td colspan=\"2\">" + device_hostname + "</td> </tr> <tr> <td>IP address: </td><td colspan=\"2\">" + device_ip_address + "</td> </tr> ";
  ptr += "<tr> <td>Subnet: </td> <td colspan=\"2\">" + device_subnet + "</td> </tr> <tr> <td>Gateway: </td> <td colspan=\"2\">" + device_gateway + "</td> </tr> ";
  ptr += "<tr> <td>MAC Address: </td> <td colspan=\"2\">" + device_macaddress + "</td> </tr> <tr> <td>Server IP Address: </td> <td colspan=\"2\">" + server_url + "</td> </tr>";
  ptr += "<tr> <td>Server Status: </td> <td colspan=\"2\">" + server_connection_status + "</td> </tr> <tr> <td>API Key: </td> <td colspan=\"2\">" + server_api_key + "</td> </tr> ";
  ptr += "<tr> <td>Connection Status: </td> <td colspan=\"2\">Authenticated</td> </tr> <tr> <td>GPS : </td> <td>" + gps_latitude + ", " + gps_longitude + "</td> <td><a href=\"https://www.google.com/maps/@" + gps_latitude + "," + gps_longitude + ",15z\" target=\"_blank\">Open in maps</a></td> ";
  ptr += "</tr> <tr> <td colspan=\"3\"><button onclick = \"window.location.reload();\">Refresh</button></td> </tr> </table>\n";
  ptr += "<h2>Device Settings</h2>\n";
  ptr += "<table> <tr> <td>Server IP:</td> <td><input type=\"text\"></td> <td><button>Change</button> </td> </tr>\n";
  ptr +="</table>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
