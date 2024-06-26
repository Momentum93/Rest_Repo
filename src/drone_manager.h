#ifndef DRONEMANAGER_H
#define DRONEMANAGER_H

#include <vector>
#include <string>
#include <list>

#include "drone.h"
#include "utils.h"

class DroneManager {
public:
    // Method to get a drone by ID
    Drone* getDroneById(const std::string& drone_id);

    // Method to get a drone by User ID
    Drone* getDroneByUserId(const std::string& user_id);

    // Method to create drones in predefined positions
    void createDrones();

    // Method to create drones in a specific area
    void createDronesInArea(Position position);

private:
    std::list<Drone> drones;
};

#endif // DRONEMANAGER_H
