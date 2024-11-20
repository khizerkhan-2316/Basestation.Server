#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <sstream>
#include "DBUtils.hpp" 
#include <./models/MeasurementData.hpp> 

class MeasurementRepository {
private:
    DBUtils dbUtils;

    // Convert MeasurementData to a map for serialization
    std::map<std::string, std::string> serialize(const MeasurementData& measurement) {
        std::map<std::string, std::string> record;
        record["temperature"] = std::to_string(measurement.getTemperature());
        record["oxygen"] = std::to_string(measurement.getOxygen());
        record["pressure"] = std::to_string(measurement.getPressure());
        record["timestamp"] = measurement.getFormattedTimestamp();
        record["isMeasurementValid"] = measurement.getIsMeasurementValid() ? "true" : "false";
        return record;
    }

    // Convert a map back to a MeasurementData object
    MeasurementData deserialize(const std::map<std::string, std::string>& record) {
        float temperature = std::stof(record.at("temperature"));
        float oxygen = std::stof(record.at("oxygen"));
        float pressure = std::stof(record.at("pressure"));
        bool isValid = (record.at("isMeasurementValid") == "true");

        MeasurementData measurement(temperature, oxygen, pressure);
        measurement.setIsMeasurementValid(isValid);
        return measurement;
    }

public:
    explicit MeasurementRepository(const std::string& filePath) : dbUtils(filePath) {}

    void saveMeasurement(const MeasurementData& measurement) {
        auto measurements = retrieveAllMeasurements(); // Retrieve existing measurements
        measurements.push_back(measurement);           // Add new measurement
        std::vector<std::map<std::string, std::string>> serializedData;
        for (const auto& m : measurements) {
            serializedData.push_back(serialize(m));
        }
        dbUtils.saveToFile(serializedData);            // Save all measurements back to file
    }

    MeasurementData retrieveMeasurementById(const std::string& id) {
        auto measurements = retrieveAllMeasurements();
        for (const auto& measurement : measurements) {
            if (measurement.getFormattedTimestamp() == id) { // Assuming timestamp is used as ID
                return measurement;
            }
        }
        throw std::runtime_error("Measurement not found with id: " + id);
    }

    std::vector<MeasurementData> retrieveAllMeasurementsByAnalysisId(const std::string& analysisId) {
        auto measurements = retrieveAllMeasurements();
        std::vector<MeasurementData> result;

        for (const auto& measurement : measurements) {
            // For this implementation, we'll use a placeholder since analysisId isn't part of MeasurementData.
            // In a real implementation, the MeasurementData class would need to include an analysisId field.
        }

        return result;
    }

    void updateMeasurement(const MeasurementData& updatedMeasurement) {
        auto measurements = retrieveAllMeasurements();
        bool updated = false;

        for (auto& measurement : measurements) {
            if (measurement.getFormattedTimestamp() == updatedMeasurement.getFormattedTimestamp()) {
                measurement = updatedMeasurement;
                updated = true;
                break;
            }
        }

        if (!updated) {
            throw std::runtime_error("Measurement not found for update.");
        }

        std::vector<std::map<std::string, std::string>> serializedData;
        for (const auto& m : measurements) {
            serializedData.push_back(serialize(m));
        }
        dbUtils.saveToFile(serializedData); // Save updated data back to file
    }

    std::vector<MeasurementData> retrieveAllMeasurements() {
        std::vector<MeasurementData> measurements;
        auto records = dbUtils.readFromFile();

        for (const auto& record : records) {
            measurements.push_back(deserialize(record));
        }

        return measurements;
    }
};
