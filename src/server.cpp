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
}

void AccessPoint :: setup_rest(){
    //this -> server.on("/", std::bind(&AccessPoint::handle_rest, this));
    droneManager.createDrones();

    this -> server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        char buffer[250];
        StaticJsonDocument<250> jsonDocument;
        Utils utils;


        jsonDocument.clear();
        jsonDocument["type"] = "Gruppen Mitglied";
        jsonDocument["value"] = "Thomas";
        jsonDocument["unit"] = "Person";
        jsonDocument["random_uid"] = utils.getRandomDroneId(1);

        serializeJson(jsonDocument, buffer);
        request->send(200, "application/json", buffer);
    });

    this -> server.on("/get_drones", HTTP_GET, [this](AsyncWebServerRequest *request){
        char buffer[2048];
        StaticJsonDocument<2048> jsonDocument;
        JsonArray drones_array = jsonDocument.createNestedArray("drones");

        for (auto& drone : *this->droneManager.getDrones()) {
            JsonObject drone_json = drones_array.createNestedObject();
            drone_json["id"] = drone.getId();
            drone_json["battery"] = drone.getBattery();
            drone_json["price"] = drone.getPrice();
            JsonObject position_json = drone_json.createNestedObject("position");
            position_json["lat"] = drone.getPosition().lat;
            position_json["lng"] = drone.getPosition().lng;
        }

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

