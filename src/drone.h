#ifndef drone_h
#define drone_h

#include <string>

#include "utils.h"

class Drone {
public:
    Drone(const std::string& id, float price, const std::string& user_id, 
          float timestamp_rental_started, int battery, Position position);

    // Getter and Setter methods
    std::string getId() const;
    void setId(const std::string& id);

    float getPrice() const;
    void setPrice(float price);

    std::string getUserId() const;
    void setUserId(const std::string& user_id);

    float getTimestampRentalStarted() const;
    void setTimestampRentalStarted(float timestamp_rental_started);

    int getBattery() const;
    void setBattery(int battery);

    Position getPosition() const;
    void setPosition(Position position);

private:
    std::string id;
    float price;
    std::string user_id;
    float timestamp_rental_started;
    int battery;
    Position position;
};

#endif // DRONE_H
