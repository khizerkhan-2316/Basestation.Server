#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "MeasurementData.hpp"
#include "SpringLayer.hpp"
#include <vector>
#include <string>
#include <algorithm>

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

public:
    Analysis() : status(AnalysisStatus::Pending) {
        startTime = std::chrono::system_clock::now();
    }

    void addSpringLayer(float layerDepth) {
        springLayers.emplace_back(layerDepth);
    }

    void removeSpringLayer(const std::string& layerId) {
        springLayers.erase(
            std::remove_if(springLayers.begin(), springLayers.end(),
                           [&layerId](const SpringLayer& layer) { return layer.getId() == layerId; }),
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
        std::cout << "Analysis ID: " << id << "\nSubmarine ID: " << submarineId << "\nStatus: ";
        std::cout << (status == AnalysisStatus::Pending ? "Pending" : status == AnalysisStatus::InProgress ? "In Progress" : "Completed") << "\n";

        std::cout << "\nMeasurements:\n";
        for (const auto& measurement : measurements) {
            std::cout << "Temp: " << measurement.getTemperature() << " °C, "
                      << "Oxygen: " << measurement.getOxygen() << " %, "
                      << "Pressure: " << measurement.getPressure() << " atm\n";
        }

        std::cout << "\nSpring Layers:\n";
        for (const auto& layer : springLayers) {
            std::cout << "Layer Depth: " << layer.getLayerDepth() << " meters, ID: " << layer.getId() << "\n";
        }
    }
};

#endif