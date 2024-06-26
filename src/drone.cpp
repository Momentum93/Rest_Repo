#include "drone.h"
#include "utils.h"

// Constructor
Drone::Drone(
    const std::string& id, 
    float price, 
    const std::string& user_id, 
    float timestamp_rental_started,
    int battery, 
    Position position
) : id(id), price(price), user_id(user_id), timestamp_rental_started(timestamp_rental_started),
      battery(battery), position(position) {}

// Getter and Setter methods

std::string Drone::getId() const {
    return id;
}

void Drone::setId(const std::string& id) {
    this->id = id;
}

float Drone::getPrice() const {
    return price;
}

void Drone::setPrice(float price) {
    this->price = price;
}

std::string Drone::getUserId() const {
    return user_id;
}

void Drone::setUserId(const std::string& user_id) {
    this->user_id = user_id;
}

float Drone::getTimestampRentalStarted() const {
    return timestamp_rental_started;
}

void Drone::setTimestampRentalStarted(float timestamp_rental_started) {
    this->timestamp_rental_started = timestamp_rental_started;
}

int Drone::getBattery() const {
    return battery;
}

void Drone::setBattery(int battery) {
    this->battery = battery;
}

Position Drone::getPosition() const {
    return position;
}

void Drone::setPosition(Position position) {
    this->position = position;
}
