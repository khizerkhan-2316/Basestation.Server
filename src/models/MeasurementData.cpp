#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>

class MeasurementData {
private:
    float temperature;
    float oxygen;
    float pressure;
    std::chrono::system_clock::time_point timestamp;
    bool isMeasurementValid;

public:
    // Constructor to initialize the measurement data
    MeasurementData(float temp, float oxygen, float pressure)
        : temperature(temp), oxygen(oxygen), pressure(pressure), isMeasurementValid(true) {
        // Set current time as timestamp
        timestamp = std::chrono::system_clock::now();
    }

    // Getter for temperature
    float getTemperature() const {
        return temperature;
    }

    // Getter for oxygen
    float getOxygen() const {
        return oxygen;
    }

    // Getter for pressure
    float getPressure() const {
        return pressure;
    }

    // Getter for isMeasurementValid
    bool getIsMeasurementValid() const {
        return isMeasurementValid;
    }

    // Setter for isMeasurementValid
    void setIsMeasurementValid(bool isValid) {
        isMeasurementValid = isValid;
    }

    // Method to get formatted timestamp as a string
    std::string getFormattedTimestamp() const {
        auto timeT = std::chrono::system_clock::to_time_t(timestamp);
        std::tm* tmPtr = std::localtime(&timeT);
        std::ostringstream oss;
        oss << std::put_time(tmPtr, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    // Method to display the measurement data
    void display() const {
        std::cout << "Timestamp: " << getFormattedTimestamp() << std::endl;
        std::cout << "Temperature: " << temperature << " °C" << std::endl;
        std::cout << "Oxygen: " << oxygen << " %" << std::endl;
        std::cout << "Pressure: " << pressure << " atm" << std::endl;
        std::cout << "Measurement Valid: " << (isMeasurementValid ? "Yes" : "No") << std::endl;
    }
};