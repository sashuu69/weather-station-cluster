/*
 * Project Name: Home Weather Station Mini
 * Program Name: NodeMCU code
 * Created on: 20/11/2020 02:11:00 AM
 * Last Modified: 28/12/2020 09:27:00 AM
 * Created by: Sashwat K
 */

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> 
#define WEB_PORT 80

#include "DHT.h"
#define DHTPIN D5
#define DHTTYPE DHT22

#include "BMP280.h"
#include "Wire.h"
#define P0 1013.25

#include <EEPROM.h>

WiFiManager wifiManager;
ESP8266WebServer server(WEB_PORT);
WiFiClient client;
DHT dht(DHTPIN, DHTTYPE);
BMP280 bmp;

String admin_username = "admin";
String admin_password;
String server_url;
String server_api_key;
String gps_longitude;
String gps_latitude;

int timer = 0;
int addr = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  EEPROM.begin(512);
  
  readDataFromEEPROM();
  
  dht.begin();
  
  bmp.begin();
  bmp.setOversampling(4);
  
  wifiManager.autoConnect("Home Weather Station Mini");

  ArduinoOTA.setPort(8266);
  ArduinoOTA.setHostname("Home Weather Station Mini");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  
  server.on("/", handle_OnConnect);
  server.on("/change_hostname", change_hostname);
  server.on("/change_server_ip", change_server_ip);
  server.on("/change_api_key", change_api_key);
  server.on("/change_lattitude", change_lattitude);
  server.on("/change_longitude", change_longitude);
  server.on("/change_admin_password", change_admin_password);
  server.on("/reset_network", reset_network);
  server.on("/reset_everything", reset_everything);
  server.onNotFound(handleNotFound);
  server.begin();
  
  Serial.println("connected :)");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (WiFi.status() == WL_CONNECTED) {
    while(WiFi.status() == WL_CONNECTED){
      ArduinoOTA.handle();
      server.handleClient();
      float dht_humidity = dht.readHumidity();
      float dht_temperature = dht.readTemperature();
      float dht_heat_index = dht.computeHeatIndex(dht_temperature, dht_humidity, false);

      double bmp_temperature, bmp_pressure, bmp_altitude;
      char result = bmp.startMeasurment();

      if (result != 0) {
        delay(result);
        result = bmp.getTemperatureAndPressure(bmp_temperature,bmp_pressure);

        if(result!=0) {
          bmp_altitude = bmp.altitude(bmp_pressure,P0);
          if (timer % 30 == 0) {
            print_diagnostic_data(bmp_temperature, bmp_pressure, bmp_altitude, dht_humidity, dht_temperature, dht_heat_index); 
          }
        }
      }
      timer++;
      delay(1000);
    }
  }
}

void readDataFromEEPROM() {
  admin_password = read_String(200);
  server_url = read_String(250);
  server_api_key = read_String(300);
  gps_longitude = read_String(350);
  gps_latitude = read_String(400);
}

String read_String(char add) {
  int i;
  char data[100]; //Max 100 Bytes
  int len=0;
  unsigned char k;
  k=EEPROM.read(add);
  while(k != '\0' && len<500)   //Read until null character
  {    
    k=EEPROM.read(add+len);
    data[len]=k;
    len++;
  }
  data[len]='\0';
  return String(data);
}

void writeString(char add,String data) {
  int _size = data.length();
  int i;
  for (i = 0; i < 50; i++) {
    EEPROM.write(add+i, 0);
  }
  EEPROM.end();
  for(i=0;i<_size;i++)
  {
    EEPROM.write(add+i,data[i]);
  }
  EEPROM.write(add+_size,'\0');   //Add termination null character for String Data
  EEPROM.commit();
}

void handle_OnConnect() {
  server.send(200, "text/html", dashboard());
}

void change_hostname() {
  WiFi.hostname(server.arg("device_hostname"));
  delay(10);
  jump_to_home();
}

void change_server_ip() {
  writeString(250, server.arg("server_ip"));
  delay(10);
  server_url = server.arg("server_ip");
  jump_to_home();
}

void change_api_key() {
  writeString(300, server.arg("api_key"));
  delay(10);
  server_api_key = server.arg("api_key");
  jump_to_home();
}

void change_lattitude() {
  writeString(350, server.arg("change_latitude"));
  delay(10);
  gps_latitude = server.arg("change_latitude");
  jump_to_home();
}

void change_longitude() {
  writeString(400, server.arg("change_longitude"));
  delay(10);
  gps_longitude = server.arg("change_longitude");
  jump_to_home();
}

void change_admin_password() {
  if (admin_password == server.arg("old_password")) {
    if (server.arg("new_password") == server.arg("confirm_new_password")) {
      writeString(200, server.arg("new_password"));
      delay(10);
      admin_password = server.arg("new_password");
      jump_to_home();
    }
    else {
      server.send(200, "text/html", "<script>if(window.confirm(\"New Password not same.\")){document.location.href=\"/\";}</script>");
    }
  }
  else {
    server.send(200, "text/html", "<script>if(window.confirm(\"Old Password incorrect.\")){document.location.href=\"/\";}</script>");
  }
}

void reset_network() {
  wifiManager.resetSettings();
  ESP.restart();
}

void reset_everything() {
  admin_username = "admin";
  admin_password = "password";
  server_url = "sashwat.in";
  server_api_key = "-";
  gps_longitude = "0";
  gps_latitude = "0";
  reset_network();
}

void jump_to_home() {
    server.send(200, "text/html", "<script>document.location.href=\"/\";</script>");
}

void handleNotFound() {
  server.send(404, "text/html",the_404_page());
}

void writeString(int address, String data)
{
  int _size = data.length();
  for(int i=0;i<_size;i++)
  {
    EEPROM.write(address+i, data[i]);
  }
  EEPROM.write(address + _size,'\0');   //Add termination null character
}

String readString(int address)
{
  char data[100]; //Max 100 Bytes
  int len=0;
  unsigned char k;
  k = EEPROM.read(address);
  while(k != '\0' && len < 100)   //Read until null character
  {
    k = EEPROM.read(address + len);
    data[len] = k;
    len++;
  }
  data[len]='\0';
  return String(data);
}

String dashboard() {
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
  ptr += "<table> <tr> <td>Hostname</td> <td> <form action=\"/change_hostname\" method=\"POST\"> <input type=\"text\" name=\"device_hostname\"></td> <td><button onclick=\"return confirm('Are you sure?');\">Change</button></form></td> </tr>\n";
  ptr += "<tr> <td>Server IP:</td> <td> <form action=\"/change_server_ip\" method=\"POST\"><input type=\"text\" name=\"server_ip\"></td> <td><button onclick=\"return confirm('Are you sure?');\">Change</button></form></td> </tr>\n";
  ptr += "<tr> <td>API Key: </td> <td> <form action=\"/change_api_key\" method=\"POST\"> <input type=\"text\" name=\"api_key\"></td> <td><button onclick=\"return confirm('Are you sure?');\">Change</button></form></td> </tr>\n";
  ptr += "<tr> <td>GPS Lattitude: </td> <td> <form action=\"/change_lattitude\" method=\"POST\"> <input type=\"text\" name=\"change_latitude\"> </td> <td> <button onclick=\"return confirm('Are you sure?');\">Change</button> </form> </td>\n";
  ptr += "<tr> <td>GPS Longitude: </td> <td> <form action=\"/change_longitude\" method=\"POST\"> <input type=\"text\" name=\"change_longitude\"> </td> <td> <button onclick=\"return confirm('Are you sure?');\">Change</button> </form> </td> </table>\n";
  ptr += "<h2>Admin Settings</h2>\n";
  ptr += "<form action=\"/change_admin_password\" method=\"POST\"> <table> <tr> <td>Old password</td> <td><input type=\"password\" name=\"old_password\"></td> </tr> <tr> <td>New password</td> <td><input type=\"password\" name=\"new_password\"> </td> </tr>\n";
  ptr += "<tr> <td>Confirm new password</td> <td><input type=\"password\" name=\"confirm_new_password\"></td> </tr> <tr> <td colspan=\"2\"> <button onclick=\"return confirm('Are you sure?');\">Change</button></td> </tr> </table> </form>\n";
  ptr += "<h2>Reset</h2>\n";
  ptr += "<table> <tr> <td> Reset Network </td> <td> <form action=\"/reset_network\" method=\"POST\"> <button onclick=\"return confirm('Are you sure?');\">Submit</button> </form> </td> </tr>\n";
  ptr += "<tr> <td> All </td> <td> <form action=\"/reset_everything\" method=\"POST\"> <button onclick=\"return confirm('Are you sure?');\">Submit</button> </form> </td> </tr> </table>";
  ptr += "</body>\n";
  ptr += "</html>\n";
  
  return ptr;
}

String the_404_page() {
  String ptr = "<!DOCTYPE html> <html>\n";

  ptr += "<head> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>Home Weather Station Mini</title>\n";
  ptr += "<link href=\"https://fonts.googleapis.com/css2?family=Nunito+Sans:wght@600;900&display=swap\" rel=\"stylesheet\">\n";
  ptr += "<style>body {background-color: #95c2de;}\n";
  ptr += ".mainbox {background-color: #95c2de;margin: auto;height: 600px;width: 600px;position: relative;}\n";
  ptr += ".err {color: #ffffff;font-family: 'Nunito Sans', sans-serif;font-size: 11rem;position:absolute;left: 20%;top: 8%;}\n";
  ptr += ".far {position: absolute;font-size: 8.5rem;left: 42%;top: 15%;color: #ffffff;}\n";
  ptr += ".err2 {color: #ffffff;font-family: 'Nunito Sans', sans-serif;font-size: 11rem;position:absolute;left: 68%;top: 8%;}\n";
  ptr += ".msg {text-align: center;font-family: 'Nunito Sans', sans-serif;font-size: 1.6rem;position:absolute;left: 16%;top: 45%;width: 75%;}\n";
  ptr += "a {text-decoration: none; color: white;}\n";
  ptr += "a:hover {text-decoration: underline;}</style>\n";
  ptr += "<script src=\"https://kit.fontawesome.com/4b9ba14b0f.js\" crossorigin=\"anonymous\"></script>\n";
  ptr += "</head> <body>\n";
  ptr += "<div class=\"mainbox\"><div class=\"err\">4</div><i class=\"far fa-question-circle fa-spin\"></i><div class=\"err2\">4</div>\n";
  ptr += "<div class=\"msg\">Maybe this page moved? Got deleted? Is hiding out in quarantine? Never existed in the first place?<p>Let's go <a href=\"/\">home</a> and try from there.</p></div>";
  ptr += "</div></body></html>\n";
  
  return ptr;
}

void print_diagnostic_data(double bmp_temperature, double bmp_pressure, double bmp_altitude, float dht_humidity, float dht_temperature, float dht_heat_index) {
  Serial.println("\n=============================================");
  Serial.println("=============================================\n");
  Serial.println("WiFi Details");
  Serial.println("============");
  Serial.print("SSID: ");Serial.println(WiFi.SSID());
  Serial.print("IP address: ");Serial.println(WiFi.localIP());
  Serial.print("Subnet: ");Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");Serial.println(WiFi.gatewayIP());

  Serial.println("\nDevice Details");
  Serial.print("MAC address: ");Serial.println(WiFi.macAddress());
  Serial.print("Hostname: ");Serial.println(WiFi.hostname());
  
  Serial.println("\nBMP280 Sensor");
  Serial.println("===============");
  Serial.print("Temperature: ");Serial.println(bmp_temperature);
  Serial.print("Pressure: ");Serial.println(bmp_pressure);
  Serial.print("Altitude: ");Serial.println(bmp_altitude);
  
  Serial.println("\nDHT22 Sensor");
  Serial.println("============");
  Serial.print("Humidity: ");Serial.println(dht_humidity);
  Serial.print("Temperature: ");Serial.println(dht_temperature);
  Serial.print("dht_heat_index: ");Serial.println(dht_heat_index);
  
  Serial.println("\n=============================================");
  Serial.println("=============================================\n");
}
