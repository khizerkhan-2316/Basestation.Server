#include <iostream>
#include <string>

class Submarine {
public:
    std::string id;
    std::string name;
    bool isAvailable;
    float length;
    float width;
    float weight;

    // Constructor
    Submarine(std::string id, std::string name, bool isAvailable, float length, float width, float weight)
        : id(id), name(name), isAvailable(isAvailable), length(length), width(width), weight(weight) {}

    // Method to display submarine details
    void display() const {
        std::cout << "Submarine ID: " << id << std::endl;
        std::cout << "Name: " << name << std::endl;
        std::cout << "Available: " << (isAvailable ? "Yes" : "No") << std::endl;
        std::cout << "Length: " << length << " cm" << std::endl;
        std::cout << "Width: " << width << " cm" << std::endl;
        std::cout << "Weight: " << weight << " kg" << std::endl;
    }
};
