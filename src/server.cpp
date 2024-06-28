#include "server.h"
#include "utils.h"

AccessPoint :: AccessPoint(
    uint8_t (& img)[], 
    ArrayList<Data> & L_fromPC,
    ArrayList<Data> & L_toPC) :

    //----------------------------------

    SSID("my esp32 rest ap"),
    server(80),
    img(img),
    L_fromPC(L_fromPC),
    L_toPC(L_toPC)
{
    //this->L_fromPC = L_fromPC;
    //this->img = img;
    //(*this).img = {};
    //mempcpy(&img[0], (*this).img, img.size());
};

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
    //droneManager.createDrones();
    this -> droneManager.createDrones();

    this -> server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        char buffer[250];
        StaticJsonDocument<250> jsonDocument;

        jsonDocument.clear();
        jsonDocument["api"] = "AboveYouDrone";
        jsonDocument["version"] = "1.0";
        jsonDocument["date"] = "2024-07-02";

        serializeJson(jsonDocument, buffer);
        request->send(200, "application/json", buffer);
    });

    // https://raphaelpralat.medium.com/example-of-json-rest-api-for-esp32-4a5f64774a05
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
        jsonDocument.clear();

        if (the_drone != nullptr) {
            jsonDocument["status"] = "error";
            jsonDocument["message"] = "You are already renting a drone";
        } else {
            the_drone = &(*this->droneManager.getDroneById(drone_id));  // (TODO: add null check)
            if (the_drone->getUserId() != "") {
                jsonDocument["status"] = "error";
                jsonDocument["message"] = "This drone is already being rented";
            } else {
                int current_timestamp = Utils::getCurrentTime();
                the_drone->setUserId(user_id);
                the_drone->setTimestampRentalStarted(current_timestamp);

                jsonDocument["status"] = "success";
                jsonDocument["user_id"] = user_id;
                jsonDocument["drone_id"] = drone_id;
                jsonDocument["timestamp_rental_started"] = current_timestamp;
            }
        }

        serializeJson(jsonDocument, buffer);
        request->send(200, "application/json", buffer);
    });

    AsyncCallbackJsonWebHandler *handler3 = new
    AsyncCallbackJsonWebHandler("/stop_rental", [this](AsyncWebServerRequest *request, JsonVariant &json) {
        StaticJsonDocument<1024> data = json.as<JsonObject>();

        std::string user_id = data["user_id"];
        std::string drone_id = data["drone_id"];

        Drone* the_drone = &(*this->droneManager.getDroneByUserId(user_id));
        char buffer[256];
        StaticJsonDocument<256> jsonDocument;
        jsonDocument.clear();

        if (the_drone == nullptr || the_drone->getId() != drone_id) {
            jsonDocument["status"] = "error";
            jsonDocument["message"] = "Drone not found";
        } else {
            float timestamp_rental_started = the_drone->getTimestampRentalStarted();
            int current_timestamp = Utils::getCurrentTime();
            float time_of_rental = current_timestamp - timestamp_rental_started;
            float p = time_of_rental / 60 * the_drone->getPrice();
            float price_to_pay = roundf(p * 100.0f) / 100.0f;  // only keep two decimals

            the_drone->setUserId("");
            the_drone->setTimestampRentalStarted(-1);

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
        jsonDocument.clear();

        if (the_drone == nullptr) {
            jsonDocument["active_rental"] = "no";
        } else {
            int current_timestamp = Utils::getCurrentTime();
            jsonDocument["active_rental"] = "yes";
            jsonDocument["user_id"] = user_id;
            jsonDocument["drone_id"] = the_drone->getId();
            jsonDocument["timestamp_rental_started"] = the_drone->getTimestampRentalStarted();
            jsonDocument["timestamp_diff"] = the_drone->getTimestampRentalStarted() - current_timestamp;
        }

        serializeJson(jsonDocument, buffer);
        request->send(200, "application/json", buffer);
    });

    AsyncCallbackJsonWebHandler *handler5 = new
    AsyncCallbackJsonWebHandler("/drone_follow", [this](AsyncWebServerRequest *request, JsonVariant &json) {
        StaticJsonDocument<1024> data = json.as<JsonObject>();

        std::string user_id = data["user_id"];
        std::string drone_id = data["drone_id"];

        Drone* the_drone = &(*this->droneManager.getDroneByUserId(user_id));
        char buffer[256];
        StaticJsonDocument<256> jsonDocument;
        jsonDocument.clear();

        if (the_drone == nullptr || the_drone->getId() != drone_id) {
            jsonDocument["status"] = "error";
            jsonDocument["message"] = "Wrong drone id";
        } else {
            // TODO: Send command

            jsonDocument["status"] = "success";
            jsonDocument["message"] = "Drone will continue following";
        }

        serializeJson(jsonDocument, buffer);
        request->send(200, "application/json", buffer);
    });

    AsyncCallbackJsonWebHandler *handler6 = new
    AsyncCallbackJsonWebHandler("/stop_drone_follow", [this](AsyncWebServerRequest *request, JsonVariant &json) {
        StaticJsonDocument<1024> data = json.as<JsonObject>();

        std::string user_id = data["user_id"];
        std::string drone_id = data["drone_id"];

        Drone* the_drone = &(*this->droneManager.getDroneByUserId(user_id));
        char buffer[256];
        StaticJsonDocument<256> jsonDocument;
        jsonDocument.clear();

        if (the_drone == nullptr || the_drone->getId() != drone_id) {
            jsonDocument["status"] = "error";
            jsonDocument["message"] = "Wrong drone id";
        } else {
            // TODO: Send command

            jsonDocument["status"] = "success";
            jsonDocument["message"] = "Following will be stopped";
        }

        serializeJson(jsonDocument, buffer);
        request->send(200, "application/json", buffer);
    });

    this -> server.addHandler(handler1);
    this -> server.addHandler(handler2);
    this -> server.addHandler(handler3);
    this -> server.addHandler(handler4);
    this -> server.addHandler(handler5);
}

void AccessPoint :: bind_print(void (*func)(String str)){
    this->_print = func;
}

void AccessPoint :: print(String str){
    if (this->_print){
        this->_print(str);
    }
}

