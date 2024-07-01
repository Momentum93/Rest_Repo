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

uint8_t  img[726]  PROGMEM; //ToDo need to be dynamic
uint8_t * img_pointer PROGMEM;

ArrayList<String> L_fromPC_old;
ArrayList<String> L_toPC_old;
ArrayList<Data> L_fromPC={};
ArrayList<Data> L_toPC={};

AccessPoint ap; //img, L_fromPC, L_toPC);

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
    /*
    
    const char* str = (Serial.readString()).c_str();
    char * s; 
    strcpy(s, str);
    strtok(s, ">");

    String test = "";
    */
    int i_del = tmp.indexOf('>');
    String type = tmp.substring(1,i_del);
    String payload = tmp.substring(i_del+1, tmp.length());
    //Serial.println(type + "  --  " + payload);
    if (type == "IMAGE"){
      //uint8_t t[] PROGMEM={};
      std::vector<uint8_t> myVector(payload.begin(), payload.end());
      //img_pointer = &myVector[0];

      //uint8_t t[] PROGMEM = {};
      //t = std::copy(std::begin(img_pointer), std::end(img_pointer), std::begin(dest));
      mempcpy(&myVector[0], img, myVector.size());
      
      //L_fromPC.add(Data {type, p});
    }
    else{
      L_fromPC.add(Data {type, payload});
    }
    
    //Serial.println(payload);
    /*
    const char* key; // = ("<IMAGE>").c_str();
    key = "<IMAGE>";


    if (strstr(str, key) != NULL){}
    */

   //https://stackoverflow.com/questions/1085083/regular-expressions-in-c-examples
   //std::smatch match;
   //bool b = std::regex_search(str, match, regex);
   //if (std::regex_search(test, match, std::regex(""))) {}
    //L_fromPC.add(Serial.readString());


    //for (int i = 0; i < L_fromPC.size();i++){Serial.println(L_fromPC.get(i));}

  }else if(L_toPC.size() > 0){
    Data tmp = L_toPC.get(0);
    
    Serial.println("<"+tmp.type + ">"+tmp.payload);
    L_toPC.remove(0);
  }
  
  //Serial.write((byte*)&msg, sizeof(msg));
  //Serial.println("<MESSAGE>DRONE_START");
  //Serial.write((char*)&msg, sizeof(msg));
}

