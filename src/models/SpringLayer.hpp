#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
class SpringLayer {
private:
    std::string id;
    float layerDepth;
std::chrono::system_clock::time_point timestamp;
    bool isSpringLayerValid;
public:
    // Constructor to initialize SpringLayer with layer depth
    SpringLayer(float layerDepth)
        : layerDepth(layerDepth), isSpringLayerValid(true) {
        // Generate a unique ID and set the current timestamp
        id = generateUniqueId();
        timestamp = std::chrono::system_clock::now();
    }
    // Method to generate a unique ID (for simplicity, using current time as a basis)
    static std::string generateUniqueId() {
        auto now = std::chrono::system_clock::now();
        auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(         now.time_since_epoch()).count();
        return "SL" + std::to_string(now_ms);
    }
    // Getter for layer ID
    std::string getId() const {
        return id;
    }

    // Getter for layer depth
    float getLayerDepth() const {
        return layerDepth;
    }
    // Getter for isSpringLayerValid
    bool getIsSpringLayerValid() const {
        return isSpringLayerValid;
    }
    // Setter for isSpringLayerValid
    void setIsSpringLayerValid(bool isValid) {
        isSpringLayerValid = isValid;
    }
    // Method to get formatted timestamp as a string
    std::string getFormattedTimestamp() const {
        auto timeT = std::chrono::system_clock::to_time_t(timestamp);
        std::tm* tmPtr = std::localtime(&timeT);
        std::ostringstream oss;
        oss << std::put_time(tmPtr, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }
    // Method to display the details of the SpringLayer
    void display() const {
        std::cout << "Spring Layer ID: " << id << std::endl;
        std::cout << "Layer Depth: " << layerDepth << " meters" << std::endl;
        std::cout << "Timestamp: " << getFormattedTimestamp() << std::endl;
      std::cout << "Layer Valid: " << (isSpringLayerValid ? "Yes" : "No") << std::endl;
}
};