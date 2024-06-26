#include "server.h"
#include "utils.h"

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

    this -> print((String)(*this). ip);

    this -> setup_rest();

    this -> server.begin();

    droneManager.createDrones();
}

void AccessPoint :: setup_rest(){
    //this -> server.on("/", std::bind(&AccessPoint::handle_rest, this));

    this -> server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        char buffer[250];
        StaticJsonDocument<250> jsonDocument;
        Utils utils;

        jsonDocument.clear();
        jsonDocument["type"] = "Gruppen Mitglied";
        jsonDocument["value"] = "Thomas";
        jsonDocument["unit"] = "Person";
        jsonDocument["random_uid"] = utils.getRandomDroneId();

        serializeJson(jsonDocument, buffer);
        request->send(200, "application/json", buffer);
    });

    //https://raphaelpralat.medium.com/example-of-json-rest-api-for-esp32-4a5f64774a05
    AsyncCallbackJsonWebHandler *handler = new
    AsyncCallbackJsonWebHandler("/", [](AsyncWebServerRequest *request, JsonVariant &json) {
        StaticJsonDocument<200> data;
        if (json.is<JsonArray>()) {
            data = json.as<JsonArray>();
        } else if (json.is<JsonObject>()) {
            data = json.as<JsonObject>();
        }
        String response;
        serializeJson(data, response);
        request->send(200, "application/json", response);
    });

    this -> server.addHandler(handler);
}

void AccessPoint :: bind_print(void (*func)(String str)){
    this->_print = func;
}

void AccessPoint :: print(String str){
    if (this->_print){
        this->_print(str);
    }
}

