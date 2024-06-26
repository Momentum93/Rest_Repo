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

        jsonDocument.clear();
        jsonDocument["type"] = "Gruppen Mitglied";
        jsonDocument["value"] = "Thomas";
        jsonDocument["unit"] = "Person";

        serializeJson(jsonDocument, buffer);
        request->send(200, "application/json", buffer);
    });

    https://raphaelpralat.medium.com/example-of-json-rest-api-for-esp32-4a5f64774a05
    AsyncCallbackJsonWebHandler *handler1 = new
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

    this -> server.on("/get_drones", HTTP_GET, [this](AsyncWebServerRequest *request){
        char buffer[2048];
        StaticJsonDocument<2048> jsonDocument;
        JsonArray drones_array = jsonDocument.createNestedArray("drones");

        for (auto& drone : *this->droneManager.getDrones()) {
            JsonObject drone_json = drones_array.createNestedObject();
            drone_json["id"] = drone.getId();
            drone_json["battery"] = drone.getBattery();
            drone_json["price"] = drone.getPrice();
            drone_json["user_id"] = drone.getUserId();
            drone_json["time_stamp_rental_started"] = drone.getTimestampRentalStarted();
            drone_json["price"] = drone.getPrice();
            JsonObject position_json = drone_json.createNestedObject("position");
            position_json["lat"] = drone.getPosition().lat;
            position_json["lng"] = drone.getPosition().lng;
        }

        serializeJson(jsonDocument, buffer);
        request->send(200, "application/json", buffer);
    });

    AsyncCallbackJsonWebHandler *handler2 = new
    AsyncCallbackJsonWebHandler("/rent_drone", [this](AsyncWebServerRequest *request, JsonVariant &json) {
        StaticJsonDocument<1024> data = json.as<JsonObject>();

        std::string user_id = data["user_id"];
        std::string drone_id = data["drone_id"];

        Drone* the_drone = &(*this->droneManager.getDroneByUserId(user_id));
        char buffer[256];
        StaticJsonDocument<256> jsonDocument;

        if (the_drone != nullptr) {
            jsonDocument.clear();
            jsonDocument["status"] = "error";
            jsonDocument["message"] = "You are already renting a drone";
        } else {
            the_drone = &(*this->droneManager.getDroneById(drone_id));  // (TODO: add null check)
            if (the_drone->getUserId() != "") {
                jsonDocument.clear();
                jsonDocument["status"] = "error";
                jsonDocument["message"] = "This drone is already being rented";
            } else {
                int timestamp_rental_started = Utils::getCurrentTime();
                the_drone->setUserId(user_id);
                the_drone->setTimestampRentalStarted(timestamp_rental_started);

                jsonDocument.clear();
                jsonDocument["status"] = "success";
                jsonDocument["user_id"] = user_id;
                jsonDocument["drone_id"] = drone_id;
                jsonDocument["timestamp_rental_started"] = timestamp_rental_started;
            }
        }

        serializeJson(jsonDocument, buffer);
        request->send(200, "application/json", buffer);
    });

    AsyncCallbackJsonWebHandler *handler3 = new
    AsyncCallbackJsonWebHandler("/get_rental", [this](AsyncWebServerRequest *request, JsonVariant &json) {
        StaticJsonDocument<1024> data = json.as<JsonObject>();

        std::string user_id = data["user_id"];
        std::string drone_id = data["drone_id"];

        Drone* the_drone = &(*this->droneManager.getDroneByUserId(user_id));
        char buffer[256];
        StaticJsonDocument<256> jsonDocument;

        if (the_drone == nullptr || the_drone->getId() != drone_id) {
            jsonDocument.clear();
            jsonDocument["status"] = "error";
            jsonDocument["message"] = "Drone not found";
        } else {
            float timestamp_rental_started = the_drone->getTimestampRentalStarted();
            int current_timestamp = Utils::getCurrentTime();
            float time_of_rental = current_timestamp - timestamp_rental_started;
            float price_to_pay = time_of_rental / 60 * the_drone->getPrice();
            price_to_pay = roundf(price_to_pay * 100.0f) / 100.0f;  // only keep two decimals

            the_drone->setUserId("");
            the_drone->setTimestampRentalStarted(-1);

            jsonDocument.clear();
            jsonDocument["status"] = "success";
            jsonDocument["user_id"] = user_id;
            jsonDocument["drone_id"] = drone_id;
            jsonDocument["timestamp_rental_started"] = timestamp_rental_started;
            jsonDocument["timestamp_rental_ended"] = current_timestamp;
            jsonDocument["price_to_pay"] = price_to_pay;
        }

        serializeJson(jsonDocument, buffer);
        request->send(200, "application/json", buffer);
    });

    AsyncCallbackJsonWebHandler *handler4 = new
    AsyncCallbackJsonWebHandler("/get_rental", [this](AsyncWebServerRequest *request, JsonVariant &json) {
        StaticJsonDocument<1024> data = json.as<JsonObject>();

        std::string user_id = data["user_id"];

        Drone* the_drone = &(*this->droneManager.getDroneByUserId(user_id));
        char buffer[256];
        StaticJsonDocument<256> jsonDocument;

        if (the_drone == nullptr) {
            jsonDocument.clear();
            jsonDocument["active_rental"] = "no";
        } else {
            jsonDocument.clear();
            jsonDocument["active_rental"] = "yes";
            jsonDocument["user_id"] = user_id;
            jsonDocument["drone_id"] = the_drone->getId();
            jsonDocument["timestamp_rental_started"] = the_drone->getTimestampRentalStarted();
        }

        serializeJson(jsonDocument, buffer);
        request->send(200, "application/json", buffer);
    });

    this -> server.addHandler(handler1);
    this -> server.addHandler(handler2);
    this -> server.addHandler(handler3);
    this -> server.addHandler(handler4);
}

void AccessPoint :: bind_print(void (*func)(String str)){
    this->_print = func;
}

void AccessPoint :: print(String str){
    if (this->_print){
        this->_print(str);
    }
}

