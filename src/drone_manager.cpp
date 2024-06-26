#include <random>

#include "drone_manager.h"
#include "drone.h"
#include "utils.h"

// Method to get a drone by ID
Drone* DroneManager::getDroneById(const std::string& drone_id) {
    for (auto& drone : drones) {
        if (drone.getId() == drone_id) {
            return &drone;
        }
    }
    return nullptr;
}

// Method to get a drone by User ID
Drone* DroneManager::getDroneByUserId(const std::string& user_id) {
    for (auto& drone : drones) {
        if (drone.getUserId() == user_id) {
            return &drone;
        }
    }
    return nullptr;
}

// Method to create drones closeby in multiple predefined positions
void DroneManager::createDrones() {
    std::list<Position> positions = {
        {48.1423415209611, 11.511997110488098},
        {48.09165769491002, 11.644732285917598}
    };
    
    int i = 0;
    for (Position position : positions) {
        std::string drone_id = Utils::getRandomDroneId(i * 100);
        if (getDroneById(drone_id) == nullptr) {  // drone should not exist already
            float drone_price = 0.5;
            int battery_level = Utils::getRandomBatteryLevel(i * 100);
            Drone new_drone(drone_id, drone_price, "", -1, battery_level, position);
            drones.push_back(new_drone);
        }
        i++;
    }
}

// Gets the drones
std::list<Drone>* DroneManager::getDrones() {
    return &drones;
}
