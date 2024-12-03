#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "/home/stud/ngk/restinio/dev/sample/Basestation.Server/src/models/Analysis.hpp"
#include "DBUtils.hpp"
class AnalysisRepository {
 private:
  DBUtils dbUtils;

  // Serialize Analysis to a map for storage
  std::map<std::string, std::string> serialize(const Analysis& analysis) {
    std::map<std::string, std::string> record;

    record["id"] = analysis.getId();
    record["submarineId"] = analysis.getSubmarineId();

    // Serialize measurements
    std::ostringstream measurementsStream;
    for (const auto& measurement : analysis.getMeasurements()) {
      measurementsStream << measurement.getTemperature() << ","
                         << measurement.getOxygen() << ","
                         << measurement.getPressure() << ";";
    }
    record["measurements"] = measurementsStream.str();

    // Serialize spring layers
    std::ostringstream springLayersStream;
    for (const auto& layer : analysis.getSpringLayers()) {
      springLayersStream << layer.getId() << "," << layer.getLayerDepth()
                         << ";";
    }
    record["springLayers"] = springLayersStream.str();

    // Serialize timestamps
    record["startTime"] = analysis.getStartTimeAsString();
    record["endTime"] = analysis.getEndTimeAsString();

    // Serialize status
    record["status"] = std::to_string(static_cast<int>(analysis.getStatus()));

    return record;
  }

  // Deserialize a map back to an Analysis object
  Analysis deserialize(const std::map<std::string, std::string>& record) {
    Analysis analysis;
    analysis.setId(record.at("id"));
    analysis.setSubmarineId(record.at("submarineId"));

    // Deserialize measurements
    std::istringstream measurementsStream(record.at("measurements"));
    std::string measurementData;
    while (std::getline(measurementsStream, measurementData, ';')) {
      std::istringstream ms(measurementData);
      std::string temp, oxygen, pressure;
      std::getline(ms, temp, ',');
      std::getline(ms, oxygen, ',');
      std::getline(ms, pressure, ',');
      analysis.addMeasurement(std::stof(temp), std::stof(oxygen),
                              std::stof(pressure));
    }

    // Deserialize spring layers
    std::istringstream springLayersStream(record.at("springLayers"));
    std::string springLayerData;
    while (std::getline(springLayersStream, springLayerData, ';')) {
      std::istringstream sls(springLayerData);
      std::string layerId, layerDepth;
      std::getline(sls, layerId, ',');
      std::getline(sls, layerDepth, ',');
      SpringLayer layer(std::stof(layerDepth));
      layer.setId(layerId);
      analysis.addSpringLayer(layer.getLayerDepth());
    }

    // Deserialize timestamps
    analysis.setStartTimeFromString(record.at("startTime"));
    analysis.setEndTimeFromString(record.at("endTime"));

    // Deserialize status
    analysis.setStatus(
        static_cast<AnalysisStatus>(std::stoi(record.at("status"))));

    return analysis;
  }

 public:
  explicit AnalysisRepository(const std::string& filePath)
      : dbUtils(filePath) {}

  void saveAnalysis(const Analysis& analysis) {
    auto analyses = getAllAnalysis();
    analyses.push_back(analysis);

    std::vector<std::map<std::string, std::string>> serializedData;
    for (const auto& a : analyses) {
      serializedData.push_back(serialize(a));
    }
    dbUtils.saveToFile(serializedData);
  }

  void updateAnalysis(const Analysis& updatedAnalysis) {
    auto analyses = getAllAnalysis();
    bool updated = false;

    for (auto& analysis : analyses) {
      if (analysis.getId() == updatedAnalysis.getId()) {
        analysis = updatedAnalysis;
        updated = true;
        break;
      }
    }

    if (!updated) {
      throw std::runtime_error("Analysis not found for update.");
    }

    std::vector<std::map<std::string, std::string>> serializedData;
    for (const auto& a : analyses) {
      serializedData.push_back(serialize(a));
    }
    dbUtils.saveToFile(serializedData);
  }

  Analysis retrieveAnalysisById(const std::string& id) {
    auto analyses = getAllAnalysis();

    for (const auto& analysis : analyses) {
      if (analysis.getId() == id) {
        return analysis;
      }
    }
    throw std::runtime_error("Analysis not found with id: " + id);
  }

  std::vector<Analysis> getAllAnalysis() {
    std::vector<Analysis> analyses;
    auto records = dbUtils.readFromFile();

    for (const auto& record : records) {
      analyses.push_back(deserialize(record));
    }

    return analyses;
  }
};
