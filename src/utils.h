#ifndef utils_h
#define utils_h

#include <string>
#include <random>
#include <chrono>
#include <cmath>
#include <sstream>
#include <iomanip>

struct Position {
    double lat;
    double lng;
};

class Utils {
public:
    static std::string getRandomDroneId();
    static int getRandomBatteryLevel();
    static double getDronePrice();
    
    static Position getRandomClosebyPosition(const Position& position);
    static double getCurrentTime();
};

#endif // UTILS_H
