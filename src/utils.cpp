#include "utils.h"

// Generate a random drone ID of length 8 using alphanumeric characters
std::string Utils::getRandomDroneId() {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int length = 8;
    std::string result;
    result.reserve(length);

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(0, sizeof(alphanum) - 2);

    for (int i = 0; i < length; ++i) {
        result += alphanum[distribution(generator)];
    }

    return result;
}

// Generate a random battery level between 10 and 99
int Utils::getRandomBatteryLevel() {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(10, 99);
    return distribution(generator);
}

// Get the drone price per minute
double Utils::getDronePrice() {
    return 0.59;
}

// Get a random closeby position based on the input position
Position Utils::getRandomClosebyPosition(const Position& position) {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<> distribution(-0.0001, 0.0001);

    Position newPosition;
    newPosition.lat = position.lat + distribution(generator);
    newPosition.lng = position.lng + distribution(generator);

    return newPosition;
}

// Get the current time in seconds
double Utils::getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::duration<double>>(duration).count();
}
