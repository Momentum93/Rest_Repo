#include <Arduino.h>
#include "server.h"
#include <ArrayList.h> 

#define BAUDRATE 9600

ArrayList<String> L_fromPC;
ArrayList<String> L_toPC;

void setup() {
  Serial.begin(BAUDRATE);
}



void loop() {
  if (Serial.available()){
    L_fromPC.add(Serial.readString());
  }else if(L_toPC.size() > 0){
    Serial.println(L_toPC.get(0));
    L_toPC.remove(0);
  }

}

