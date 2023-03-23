#include "wifiset.h"

String myHtmlPage; //This variable is used to store html contents to be displayed in softAP mode
String previous_ssid;
String previous_password = "";
unsigned int espChipId = 0; //uint32_t type
String esp_ipaddress = "";
int http_status_code;     




WebServer server(80);



void create_the_webServer()                                                                                              // Function 4
{
 {
    server.on("/", []() {
       
      //Retrive the previous SSID name
      String previous_ssid;
      for (int i = 0; i < 32; ++i)                  /*This 'i' is a global variable used*/
      {
          previous_ssid += char(EEPROM.read(i));
      }

      IPAddress myIP = WiFi.softAPIP(); 
      String myIP_string = String(myIP[0]) + '.' + String(myIP[1]) + '.' + String(myIP[2]) + '.' + String(myIP[3]);
      
      String myMAC_string = WiFi.macAddress();
      espChipId = ESP.getEfuseMac();
      String myChipId_string = String(espChipId);
      
      myHtmlPage = "<!DOCTYPE HTML>\r\n<html><body bgcolor = '#008080'> <p><p><Center><strong><h1>Board Name :  " + String(BOARD_NAME)+"</h1></strong></Center><br>";
      myHtmlPage = myHtmlPage + " IP address is :     " + myIP_string + "<br>";
      myHtmlPage = myHtmlPage + " MAC address is :     " + myMAC_string + "<br>";
      myHtmlPage = myHtmlPage + " Chip ID is :     " + myChipId_string + "<br>";
      myHtmlPage += "<p>Device was previously connected to WiFi SSID : <Strong> <font color = '#003311'> ";
      myHtmlPage = myHtmlPage + previous_ssid + "</font color></strong></p>";
      myHtmlPage += "<p><font color = '#4d0000'>NOTE: Either hit 'restart' (to restart with previous WIFI credentials)     OR   <br> ";
      myHtmlPage += " Submit 'SSID' and 'PASSWORD' respectively of new active WIFI network</font color></p>";
      myHtmlPage += "<form action=\"/restart\" method=\"POST\"><input type=\"submit\" value=\"restart\"></form>";
     // myHtmlPage += "<form action=\"/live_data\" method=\"POST\"><input type=\"submit\" value=\"live_data\"></form>";
      myHtmlPage += "<p>";
      myHtmlPage += "<p>";
      myHtmlPage += "<CENTER><h2><strong>Enter New WiFi Credentials</h2></strong>";
      myHtmlPage += "</p><form method='get' action='setting'><label>WiFi SSID: </label><input name='ssid' length=32> <p> <label>Password: </label><input name='pass' length=64><p><input type='submit'></form> </CENTER>";
      myHtmlPage += "</body></html>";
      server.send(200, "text/html", myHtmlPage);
    });
    Serial.println("Inside server");

    server.on("/restart", []() {
      myHtmlPage = "<!DOCTYPE HTML>\r\n<html>OK! Device is restarting with previously used WIFI credentials";
      myHtmlPage += "</html>";
      server.send(200, "text/html", myHtmlPage);
      ESP.restart();
    });

    server.on("/live_data", []() {       
      myHtmlPage = "<!DOCTYPE HTML>\r\n<html><head><meta http-equiv=\"refresh\" content=\"10\"></head>";
      myHtmlPage += "<h2>We can see latest obtained readings here incase we dont have an internet connection. </h2>";

      myHtmlPage += "<h2>Code for live readings is not inserted here till now. Pls revisit the home page. </h2>";
      
      myHtmlPage += "</html>";
      server.send(200, "text/html", myHtmlPage);
    });



 
    server.on("/setting", []() {

      String new_ssid = server.arg("ssid");
      String new_password = server.arg("pass");

      if (new_ssid.length() > 0 && new_password.length() > 0) 
      {
        //Clearing EEPROM contents first
        for (int i = 0; i < 96; ++i) 
        {
          EEPROM.write(i, 0);
        }
        
        //Writing wifi ssid in EEPROM
        for (int i = 0; i < new_ssid.length(); ++i)
        {
          EEPROM.write(i, new_ssid[i]);
        }

        //Writing wifi password in EEPROM
        for (int i = 0; i < new_password.length(); ++i)
        {
          EEPROM.write(32 + i, new_password[i]);
        }
        /*An EEPROM write takes 3.3 ms to complete. The EEPROM memory has a specified life of 100,000 write/erase cycles*/

        /*Checks whether the write was successful or not*/
        EEPROM.commit();
 
        myHtmlPage = "EEPROM Success";
        http_status_code = 200;
        ESP.restart();
      } 
      else 
      {
        myHtmlPage = "EEPROM Fail";
        http_status_code = 404;
      }

      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(http_status_code, "application/json", myHtmlPage);
       
    });
  } 
}



// Connect to WiFi
void setUp_the_webServer()                                                                                               // Function 3
{
  
  create_the_webServer();
  // Start the server
  server.begin();                                                                                                        //Go to IP address 192.168.4.1 on web browser after connecting to BOARD_NAME
 
}

void switch_ESP_to_APmode(void)
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    WiFi.softAP(BOARD_WIFI_AP_MODE_SSID_NAME, BOARD_WIFI_AP_MODE_PASSWORD);
    
    setUp_the_webServer();
}


bool try_to_connect_to_wifi()                                                                                              //Function 2
{
  int count = 0;
 
  //Waiting 1 min for wifi to connect
  while ( count < 100 ) 
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      
      //Return True if Successfully connected to Wifi
      return true;
    }
    delay(100);

    //Total Delay = 25msx4 = 100msec for every count.
    //Thus total wait time is 100msx1200 = 120 sec = 2mins
    count++;
  }
  
  //Return False if Connection time out and then start AP
  return false;
}



void System_init()                                                                                                         // function 1
{
  
    //Disconnect any previously connected wifi
    WiFi.disconnect();
                     
    //Initialasing EEPROM to use the specified number of bytes (min=4 and max=4096)
    EEPROM.begin(512);
    delay(10);
    Serial.println(EEPROM.readULong(105));

    //EEPROM.writeULong(105,0);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);
    //Read previously connected wifi network ssid and password from EEPROM memory
    digitalWrite(BLUE_LED, GREEN_LED_OFF_STATE); 
   
    for (int i = 0; i < 32; ++i)
    {
        previous_ssid += char(EEPROM.read(i));
    }
    previous_password = "";
    for (int i = 32; i < 96; ++i)
    {
        previous_password += char(EEPROM.read(i));
    }

    //Try to begin the wifi with previously connected network
    WiFi.mode(WIFI_STA);
    WiFi.begin(previous_ssid.c_str(), previous_password.c_str());
    Serial.print("Connecting to SSID: ");
    Serial.println(previous_ssid);
    if (try_to_connect_to_wifi())
    {
      digitalWrite(GREEN_LED, GREEN_LED_OFF_STATE);
      digitalWrite(BLUE_LED, GREEN_LED_ON_STATE); 
      //Means we have successfully connected to wifi network
      //return;
    }
    else
    {
        //Unable to connect to the wifi netwok, thus turning the hotspot on
        setUp_the_webServer();
        //Start ESP as hotspot
        digitalWrite(BLUE_LED, GREEN_LED_OFF_STATE);        
        Serial.println("Connect to Wifi  1_EVD_ESP_RD");
        Serial.println("Password is apt2022esp ");
        Serial.println("Now go to 192.168.4.1 on web browser and enter your current SSID and Password");
        switch_ESP_to_APmode();
    }

    //Waiting to get connected
    while ((WiFi.status() != WL_CONNECTED))
    {
        static unsigned long ap_time_t1 = millis();
        static unsigned long ap_timeout = (300*1000);

        if((millis()-ap_time_t1) > ap_timeout)
        {
          ESP.restart();
        }
        else
        {
          for(int i=0; i<10 ; i++)
          {
            digitalWrite(GREEN_LED, GREEN_LED_OFF_STATE);
            delay(500);
            digitalWrite(GREEN_LED, GREEN_LED_ON_STATE);
            delay(500);  
          }
          server.handleClient();
        }
    }

    

    IPAddress espAddress = WiFi.localIP();
    
    char myip[40];  
    sprintf(myip, "%d.%d.%d.%d", espAddress[0], espAddress[1], espAddress[2], espAddress[3]);
    esp_ipaddress = myip;
    Serial.print("local IP address is : ");
    Serial.println(esp_ipaddress);
}


