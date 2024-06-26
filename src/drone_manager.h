#ifndef DRONEMANAGER_H
#define DRONEMANAGER_H

#include <vector>
#include <string>
#include <list>

#include "drone.h"
#include "utils.h"

class DroneManager {
public:
    Drone* getDroneById(const std::string& drone_id);
    Drone* getDroneByUserId(const std::string& user_id);
    void createDrones();
    std::list<Drone>* getDrones();

private:
    std::list<Drone> drones;
};

#endif // DRONEMANAGER_H
