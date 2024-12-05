#include <chrono>
#include <iomanip>
#include <iostream>
#include <vector>

class Measurement {
 private:
  float temperature;
  float oxygen;
  float depth;
  std::chrono::system_clock::time_point timestamp;

  template <typename JSON_IO>
  void json_io(JSON_IO& io) {
    io& json_dto::mandatory("temprature", temperature) &
        json_dto::mandatory("oxygen", oxygen) &
        json_dto::mandatory("depth", depth) &
        json_dto::mandatory("timestamp", timestamp);
  }

 public:
  // Constructor to initialize the measurement data
  Measurement(float temp, float oxygen, float depth)
      : temperature(temp), oxygen(oxygen), depth(depth) {
    // Set current time as timestamp
    timestamp = std::chrono::system_clock::now();
  }

  // Getter for temperature
  float getTemperature() const { return temperature; }

  // Getter for oxygen
  float getOxygen() const { return oxygen; }

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
    std::cout << "Depth: " << depth << " atm" << std::endl;
  }
};