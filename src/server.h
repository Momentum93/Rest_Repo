//#include <iostream>
#include <Arduino.h>
#include <string>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
//#include <WebServer.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>
//#include <AsyncTCP.h>
//#include <WiFiManager.h>

// using namespace std ?


//frei inspiriert nach:
//https://randomnerdtutorials.com/esp32-wi-fi-manager-asyncwebserver/

#ifndef AccessPoint_H
#define AccessPoint_H
class AccessPoint{
  
    private:
        char * SSID;
        //WebServer server;
        IPAddress ip;
        AsyncWebServer server;

        void setup_rest();
        void handle_rest();

    public:
        AccessPoint();
        void set_SSID(char * c);
        void start();
        IPAddress get_ip();

        //can't return Webserver server?????
        //workaround:
        void handle_client();
       
};

#endif


