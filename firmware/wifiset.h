#ifndef WIFISET_H
#define WIFISET_H
#endif


#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>    
#include <EEPROM.h>
#include <ArduinoJson.h>


extern void System_init();  

  
#define SERVER "192.168.0.105"  // Server URL (without https://www)
#define BOARD_NAME                      "1_EVD_ESP_RD"            

#define GREEN_LED                         14
#define BLUE_LED                          12
#define GREEN_LED_ON_STATE                LOW
#define GREEN_LED_OFF_STATE               HIGH
#define BOARD_WIFI_AP_MODE_SSID_NAME      BOARD_NAME                // Used when ESP goes into AP mode
#define BOARD_WIFI_AP_MODE_PASSWORD       BOARD_OTA_PASSWORD 
#define BOARD_OTA_NAME                  BOARD_NAME                // Used for OTA in ESP using Arduino IDE
#define BOARD_OTA_PASSWORD              "apt2022esp" 
extern unsigned int espChipId;

extern String esp_ipaddress;
