#include <Arduino.h>
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
}

void loop() {

}