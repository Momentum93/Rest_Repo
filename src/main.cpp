#include <Arduino.h>
#include "server.h"
#include <ArrayList.h> 

//#include <regex.h>
//#include <string>
//#include <cstddef>
//#include <iostream>
//#include <regex>
//#include <string>

#define BAUDRATE 9600

#include <stdint.h>

#include <iterator>
#include "utils.h"

/*
typedef  struct {
    String str;
    //byte* payload;
    //String payload;
    String payload;
} Data;
*/

/*
uint8_t  img[726]  PROGMEM; //ToDo need to be dynamic
uint8_t * img_pointer PROGMEM;

ArrayList<String> L_fromPC_old;
ArrayList<String> L_toPC_old;
ArrayList<Data> L_fromPC={};
ArrayList<Data> L_toPC={};
*/

String img = "";
std::vector<String> L_fromPC;
std::vector<String> L_toPC;

AccessPoint ap(L_toPC, L_fromPC, img); //img, L_fromPC, L_toPC);

/*
regex_t regex;
int reti;
*/


typedef __attribute__ ((packed)) struct{
    //char str[10];
    char foo[10];
    int value1;
    int value2;
    char str[10];
    char t[10];
    int value3;
} controlMessage;
controlMessage msg;

void setup() {
  Serial.begin(BAUDRATE);
  ap.start();
  /*
  strcpy(msg.str, "struct123");
  strcpy(msg.t, "t");
  strcpy(msg.foo, "foo");
  msg.value1=10;
  msg.value2=2030;
  msg.value3 = 999;
  */
 /*
 reti = regcomp(&regex, "^<[.]*>", REG_EXTENDED);
 if (reti){
  Serial.println("error with regex");
 }
 */
}




void loop() {

  
  if (Serial.available()){
    String tmp = Serial.readString();

    int i_del = tmp.indexOf('>');
    String type = tmp.substring(1,i_del);
    String payload = tmp.substring(i_del+1, tmp.length());
    //Serial.println(type + "  --  " + payload);
    if (type == "IMAGE"){
      img = payload;
    }
    else{
      L_fromPC.push_back(tmp);
    }
    


  }else if(L_toPC.size() > 0){
    //String str = L_toPC.front();
    Serial.println(L_toPC.front());
    L_toPC.erase(L_toPC.begin());
  }
  
}

