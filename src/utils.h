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
    static std::string getRandomDroneId(int seed);
    static int getRandomBatteryLevel(int seed);
    static double getDronePrice();
    static int getCurrentTime();
};

#endif // UTILS_H
