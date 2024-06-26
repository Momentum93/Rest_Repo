#include <iostream>
#include <string>
#include <cstdlib> // Needed for rand() and srand()
#include <ctime>   // Needed for time()

#include "utils.h"

// Generate a random drone ID of length 8 using alphanumeric characters
std::string Utils::getRandomDroneId(int seed) {
    char character_set[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int length = 6;
    std::string result;
    result.reserve(length);

    // Get a different random number each time the program runs
    srand(seed + getCurrentTime());
	for(int i=1; i <= length; i++){
        int randomNum = rand() % 37;  // 37 is the length of character_set
		result += character_set[randomNum];
	}
	
    return result;
}

// Generate a random battery level between 20 and 100
int Utils::getRandomBatteryLevel(int seed) {
    srand(seed);
    int randomNum = rand() % 81 + 20;
    return randomNum;
}

// Get the drone price per minute
double Utils::getDronePrice() {
    return 0.59;
}

// Get the current time in seconds
int Utils::getCurrentTime() {
    std::time_t t = std::time(0);
    return t;
}
