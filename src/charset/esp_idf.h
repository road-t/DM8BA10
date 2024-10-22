// This file allows the DM8BA10 library to be used with ESP-IDF without including the Arduino.h dependency
#pragma once

#include <cstdint>  // For uint16_t, uint8_t
#include <string>

typedef uint16_t word;
typedef uint8_t byte;

// std::string wrapper with substring method added for lib compatibility
class String : public std::string {
public:
    String(const std::string& str) : std::string(str) {}
    String(const char* str) : std::string(str) {}

    String substring(size_t pos = 0, size_t count = std::string::npos) const {
        return String(this->substr(pos, count));
    }
};