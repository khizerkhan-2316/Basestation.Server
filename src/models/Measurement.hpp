#pragma once

#include <chrono>
#include <iomanip>
#include <iostream>
#include <vector>
#include <sstream>
#include <ctime>
#include <json_dto/pub.hpp>  // Assuming you're using json_dto for deserialization

class Measurement {
 private:
  int id;
  std::string groupingId;
  float temperature;
  float oxygen;
  float depth;
  std::string timestamp;  // Optional field

 public:  
  // Custom serialization/deserialization for JSON
 template <typename JSON_IO>
void json_io(JSON_IO& io) {
    io & json_dto::mandatory("temperatur", temperature)
       & json_dto::mandatory("oxygen", oxygen)
       & json_dto::mandatory("dybde", depth)
       & json_dto::optional("groupingId", groupingId, "")
       & json_dto::optional("id", id, 0)
       & json_dto::optional("timestamp", timestamp, getFormattedTimestamp(std::chrono::system_clock::now()));  // Optional timestamp, default to current time
}


  // Constructor to initialize the measurement data
  Measurement(float temp, float oxygen, float depth)
      : temperature(temp), oxygen(oxygen), depth(depth) {
    // Set current time as timestamp
    timestamp = getFormattedTimestamp(std::chrono::system_clock::now());
  }

  // Default constructor
  Measurement() {
    timestamp = getFormattedTimestamp(std::chrono::system_clock::now());
  }

  // Getter methods
  float getTemperature() const { return temperature; }
  void setTemperature(float temp){ temperature = temp;}
  void setOxygen(float oxy){oxygen = oxy;}
  void setDepth(float dep) {depth = dep;}
  void setGroupingId(std::string groupdId){ groupingId = groupdId;}
  void setTimeStamp(std::string stamp){timestamp = stamp;}
  void setId(int uniqueId){id = uniqueId;}
  int getId(){return id;}
  
  float getOxygen() const { return oxygen; }
  float getDepth() const { return depth; }
  std::string getGroupingId() const { return groupingId; }
  std::string getTimeStamp() const { return timestamp; }

  // Function to format and return the timestamp as a string
  std::string getFormattedTimestamp(std::chrono::system_clock::time_point timestamp) {
    auto timeT = std::chrono::system_clock::to_time_t(timestamp);
    std::tm* tmPtr = std::localtime(&timeT);
    std::ostringstream oss;
    oss << std::put_time(tmPtr, "%Y-%m-%d %H:%M:%S");
    return oss.str();
  }
};

