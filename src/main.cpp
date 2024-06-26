#include <Arduino.h>
<<<<<<< Updated upstream
#include "server.h"

#include "serial.h"

// Default ip 192.168.4.1
AccessPoint ap;
// _Serial _serial;

void setup() {
  // _serial.register_pc_callback(
  //   [](String str){_serial.write(str);});
  // _serial.begin();

  // ap.bind_print([](String str){_serial.write(str);});
  ap.start();
=======
//#include "server.h"

#include <ArrayList.h> 

#define BAUDRATE 9600

ArrayList<String> L_fromPC;
ArrayList<String> L_toPC;

void setup() {
  Serial.begin(9600);
>>>>>>> Stashed changes
}



void loop() {
  if (Serial.available()){
    L_fromPC.add(Serial.readString());
  }else if(L_toPC.size() > 0){
    Serial.println(L_toPC.get(0));
    L_toPC.remove(0);
  }

}

