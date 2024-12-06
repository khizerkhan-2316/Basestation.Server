#ifndef GUID_GENERATOR_H
#define GUID_GENERATOR_H

#include <string>
#include <random>
#include <sstream>
#include <iomanip>

class GUIDGenerator {
public:
    // Static method to generate GUID
    static std::string generateGUID() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(0, 15);

        std::ostringstream oss;
        oss << std::hex << std::setfill('0');
        for (int i = 0; i < 8; ++i) oss << dist(gen);
        oss << "-";
        for (int i = 0; i < 4; ++i) oss << dist(gen);
        oss << "-4"; // Version 4 UUID
        for (int i = 0; i < 3; ++i) oss << dist(gen);
        oss << "-";
        oss << (8 + dist(gen) % 4); // Variant UUID
        for (int i = 0; i < 3; ++i) oss << dist(gen);
        oss << "-";
        for (int i = 0; i < 12; ++i) oss << dist(gen);

        return oss.str();
    }
};

#endif // GUID_GENERATOR_H
