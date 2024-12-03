#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <algorithm>
#include <chrono>
#include <ctime>
#include <sstream>
#include <string>
#include <vector>

#include "MeasurementData.hpp"
#include "SpringLayer.hpp"

enum class AnalysisStatus { Pending, InProgress, Completed };

class Analysis {
 private:
  std::string id;
  std::string submarineId;
  std::vector<MeasurementData> measurements;
  std::vector<SpringLayer> springLayers;
  std::chrono::system_clock::time_point startTime;
  std::chrono::system_clock::time_point endTime;
  AnalysisStatus status;

template <typename Json_Io>
  void json_io(Json_Io& io) {
    io& json_dto::mandatory("id", id) &
        json_dto::mandatory("submarineId", submarineId) &
        json_dto::mandatory("measurements", measurements) &
        json_dto::mandatory("springlayers", springLayers) &
        json_dto::mandatory("startTime", startTime) &
        json_dto::mandatory("endTime", endTime) &
        json_dto::mandatory("status", status);
  }

 public:
  Analysis() : status(AnalysisStatus::Pending) {
    startTime = std::chrono::system_clock::now();
  }

  void addSpringLayer(float layerDepth) {
    springLayers.emplace_back(layerDepth);
  }

  void removeSpringLayer(const std::string& layerId) {
    springLayers.erase(std::remove_if(springLayers.begin(), springLayers.end(),
                                      [&layerId](const SpringLayer& layer) {
                                        return layer.getId() == layerId;
                                      }),
                       springLayers.end());
  }

  SpringLayer getSpringLayer(const std::string& layerId) const {
    for (const auto& layer : springLayers) {
      if (layer.getId() == layerId) {
        return layer;
      }
    }
    throw std::runtime_error("Spring Layer not found");
  }

  void addMeasurement(float temp, float oxygen, float pressure) {
    measurements.emplace_back(temp, oxygen, pressure);
  }

  void removeMeasurement(size_t index) {
    if (index < measurements.size()) {
      measurements.erase(measurements.begin() + index);
    }
  }

  void displayAnalysis() const {
    std::cout << "Analysis ID: " << id << "\nSubmarine ID: " << submarineId
              << "\nStatus: ";
    std::cout << (status == AnalysisStatus::Pending      ? "Pending"
                  : status == AnalysisStatus::InProgress ? "In Progress"
                                                         : "Completed")
              << "\n";

    std::cout << "\nMeasurements:\n";
    for (const auto& measurement : measurements) {
      std::cout << "Temp: " << measurement.getTemperature() << " °C, "
                << "Oxygen: " << measurement.getOxygen() << " %, "
                << "Pressure: " << measurement.getPressure() << " atm\n";
    }

    std::cout << "\nSpring Layers:\n";
    for (const auto& layer : springLayers) {
      std::cout << "Layer Depth: " << layer.getLayerDepth()
                << " meters, ID: " << layer.getId() << "\n";
    }
  }

  const std::string& getId() const { return id; }

  void setId(const std::string& analysisId) { id = analysisId; }

  const std::string& getSubmarineId() const { return submarineId; }

  void setSubmarineId(const std::string& subId) { submarineId = subId; }

  const std::vector<MeasurementData>& getMeasurements() const {
    return measurements;
  }

  const std::vector<SpringLayer>& getSpringLayers() const {
    return springLayers;
  }

  void setStartTimeFromString(const std::string& timeStr) {
    std::istringstream timeStream(timeStr);
    std::time_t t;
    timeStream >> t;
    startTime = std::chrono::system_clock::from_time_t(t);
  }

  void setEndTimeFromString(const std::string& timeStr) {
    std::istringstream timeStream(timeStr);
    std::time_t t;
    timeStream >> t;
    endTime = std::chrono::system_clock::from_time_t(t);
  }

  std::string getStartTimeAsString() const {
    std::time_t t = std::chrono::system_clock::to_time_t(startTime);
    std::ostringstream timeStream;
    timeStream << t;
    return timeStream.str();
  }

  std::string getEndTimeAsString() const {
    std::time_t t = std::chrono::system_clock::to_time_t(endTime);
    std::ostringstream timeStream;
    timeStream << t;
    return timeStream.str();
  }

  void setStatus(AnalysisStatus newStatus) {
    status = newStatus;
    if (newStatus == AnalysisStatus::Completed) {
      endTime = std::chrono::system_clock::now();
    }
  }

  AnalysisStatus getStatus() const { return status; }
};
#endif