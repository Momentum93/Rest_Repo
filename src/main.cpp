#include <Arduino.h>
#include "server.h"
//-----------------------------

#define BAUD_RATE 9600
//-----------------------------

AccessPoint ap;
//-----------------------------


void printLn(String str){
  Serial.println(str);
}

void setup() {
  Serial.begin(BAUD_RATE);
  //ap.set_SSID("test");
  //IPAddress ip = ap.start();
  //Serial.println(ip);
  //ap.start();
  //Serial.println(ap.get_ip());
  ap.start();
  Serial.print(ap.get_ip());

}

void loop() {
  //ap.handle_client();
}