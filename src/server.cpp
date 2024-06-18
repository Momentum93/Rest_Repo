#include "server.h"

AccessPoint :: AccessPoint() :
    SSID("my esp32 rest ap"),
    server(80)
{};

void AccessPoint :: set_SSID(char * c){
    this -> SSID = c;
}

void AccessPoint :: start(){
    WiFi.softAP((*this).SSID);
    this -> ip = WiFi.softAPIP();

    this -> setup_rest();

    this -> server.begin();
}

IPAddress AccessPoint :: get_ip(){
    return (*this).ip;
}

void AccessPoint :: setup_rest(){
    //this -> server.on("/", std::bind(&AccessPoint::handle_rest, this));

    this -> server.on("/whos_the_best", HTTP_GET, [](AsyncWebServerRequest *request){
        char buffer[250];
        StaticJsonDocument<250> jsonDocument;

        jsonDocument.clear();
        jsonDocument["type"] = "Gruppen Mitglied";
        jsonDocument["value"] = "Thomas";
        jsonDocument["unit"] = "Person";

        serializeJson(jsonDocument, buffer);
        request->send(200, "application/json", buffer);
    });


    //Important::: will crash esp32!!!!!!!!!!!
    this -> server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request){
        StaticJsonDocument<250> jsonDocument;
        jsonDocument.clear();
        char buffer[250];

        int params = request -> params();
        for(int i=0; i<params;i++){
            AsyncWebParameter* p = request -> getParam(i);
            if (p->isPost()){
                //Serial.println(p-> name());
                //Serial.println(p->value());
                const String name = p -> name();
                const char * value = p -> value().c_str();
                jsonDocument["test"] = value;
            }
        }
        
        //AsyncWebParameter* p = request -> getParam(0);
        /*
        jsonDocument["test"] = p->name();
        serializeJson(jsonDocument, buffer);
        */
        //jsonDocument["a"] = request -> params();
        try{
            AsyncWebParameter* p = request -> getParam(0);
            jsonDocument["a"] = p->value();
            serializeJson(jsonDocument, buffer);
            request->send(200, "application/json", buffer);
        }catch(...){
            request -> send(200, "text/plain", "error");
        }
        
        

    });

    //https://raphaelpralat.medium.com/example-of-json-rest-api-for-esp32-4a5f64774a05
    AsyncCallbackJsonWebHandler *handler = new
    AsyncCallbackJsonWebHandler("/post-message", [](AsyncWebServerRequest *request, JsonVariant &json) {
        StaticJsonDocument<200> data;
        if (json.is<JsonArray>())
        {
        data = json.as<JsonArray>();
        }
        else if (json.is<JsonObject>())
        {
        data = json.as<JsonObject>();
        }
        String response;
        serializeJson(data, response);
        request->send(200, "application/json", response); 
    });

    this -> server.addHandler(handler);
}


void AccessPoint :: handle_rest(){
    //create_json("temperature", 3.3, "°C")
    char buffer[250];
    StaticJsonDocument<250> jsonDocument;

    jsonDocument.clear();
    jsonDocument["type"] = "temperature";
    jsonDocument["value"] = 3.3;
    jsonDocument["unit"] = "°C";

    serializeJson(jsonDocument, buffer);

    //this -> server.send(200, "application/json", buffer);
}

void AccessPoint :: handle_client(){
    //this -> server.handleClient();
}
