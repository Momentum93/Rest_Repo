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
    std::vector<Position> positions = {
        {48.1423415209611, 11.511997110488098},
        {48.09165769491002, 11.644732285917598}
    };
    
    for (Position position : positions) {
        createDronesInArea(position);
    }
}

// Method to create multiple drones closeby of specifiy coordinates
void DroneManager::createDronesInArea(Position position) {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(3, 6);
    
    int amount_of_drones = distribution(generator);
    for (int i = 0; i < amount_of_drones; ++i) {
        std::string drone_id = Utils::getRandomDroneId();
        float drone_price = Utils::getDronePrice();
        int battery_level = Utils::getRandomBatteryLevel();
        Position random_position = Utils::getRandomClosebyPosition(position);

        Drone new_drone(drone_id, drone_price, drone_id, 0, battery_level, random_position);
        drones.push_back(new_drone);
    }
}
