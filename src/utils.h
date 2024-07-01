#ifndef utils_h
#define utils_h

#include <string>
#include <random>
#include <chrono>
#include <cmath>
#include <sstream>
#include <iomanip>
#include "Arduino.h"

struct Position {
    double lat;
    double lng;
};

typedef  struct {
    String type;
    //byte* payload;
    //String payload;
    String payload;
} Data;

class Utils {
public:
    static std::string getRandomDroneId(int seed);
    static int getRandomBatteryLevel(int seed);
    static double getDronePrice();
    static int getCurrentTime();
};

#endif // UTILS_H
