#ifndef WIFI
#define WIFI

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <functional>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/* Put your SSID & Password */
const char* ssid = "MySon";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

bool LED_STATUS = true;

const char mainPage[] PROGMEM = R"=====(
<!DOCTYPE html> 
  <html>
  <head>
  <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
  <title> LED Control </title>
  <style> 
    html{
      font-family: Helvetica;
      display: inline-block; 
      margin: 0px auto; 
      text-align: center;
    }
  
    body{
      margin-top: 50px;
    } 
    
    h1{
      color: #444444;
      margin: 50px auto 30px;
    } 
    
    h3{
      color: #444444;
      margin-bottom: 50px;
    }
    
    .button {
      display: block;
      width: 80px;
      background-color: #1abc9c;
      border: none;
      color: white;
      padding: 13px 30px;
      text-decoration: none;
      font-size: 25px;
      margin: 0px auto 35px;
      cursor: pointer;
      border-radius: 4px;
    }
    .button-on {
      background-color: #1abc9c;
    }
    
    .button-on:active {
      background-color: #16a085;
    }
    
    .button-off {
      background-color: #34495e;
    }
    
    .button-off:active {
      background-color: #2c3e50;
    }

    p {
      font-size: 14px;
      color: #888;
      margin-bottom: 10px;
    }
  </style>
  </head>
  
  <body>
    <h1>ESP8266 Web Server</h1>
    <h3>Using Access Point(AP) Mode</h3>
    
    <p>LED Status: ON</p><a class="button button-off" href="/ledoff">OFF</a>
    <p>LED Status: OFF</p><a class="button button-on" href="/ledon">ON</a>

  </body>
  </html>
)=====";

void handle_OnConnect() {
  LED_STATUS = HIGH;
  Serial.println("LED: OFF");
  server.send(200, "text/html", (String)(mainPage)); 
}

void handle_led_on() {
  LED_STATUS = HIGH;
  Serial.println("LED: ON");
  server.send(200, "text/html", (String)(mainPage)); 
}

void handle_led_off() {
  LED_STATUS = LOW;
  Serial.println("LED: OFF");
  server.send(200, "text/html", (String)(mainPage)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}


void setupWifi() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite (LED_BUILTIN, HIGH);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  server.on("/", handle_OnConnect);
  server.on("/ledon", handle_led_on);
  server.on("/ledoff", handle_led_off);
  server.onNotFound(handle_NotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}
void loopWifi() {
  server.handleClient();
  if(LED_STATUS)
  {digitalWrite(LED_BUILTIN, HIGH);}
  else
  {digitalWrite(LED_BUILTIN, LOW);}
}

#endif