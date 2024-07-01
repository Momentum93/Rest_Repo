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
#include "ArrayList.h"

#include "drone_manager.h"

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
        DroneManager droneManager;

        void setup_rest();
        void (*_print)(String str);
        void print(String str);


        //ArrayList<Data> & L_fromPC;
        //ArrayList<Data> & L_toPC;

        //https://stackoverflow.com/questions/5724171/passing-an-array-by-reference
        //uint8_t (& img)[];

    public:
        AccessPoint(
            //uint8_t (& img)[], 
            //ArrayList<Data> & L_fromPC,
            //ArrayList<Data> & L_toPC
            );
        void set_SSID(char * c);
        void start();

        void bind_print(void (*func)(String str));
};

#endif


