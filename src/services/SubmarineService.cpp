// services/SubmarineService.cpp
#include "../models/Submarine.h"
#include <vector>

class SubmarineService {
public:
    std::vector<Submarine> getAllSubmarines() {
        return {
            Submarine("1", "Nautilus", true, 30.5f, 8.2f, 550.0f),
            Submarine("2", "Seawolf", false, 40.0f, 9.5f, 750.0f),
            Submarine("3", "Red October", true, 35.0f, 8.7f, 700.0f)
        };
    }
};
