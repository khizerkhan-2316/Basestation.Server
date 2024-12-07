#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <restinio/all.hpp>
#include "../dataaccess/MeasurementRepository.hpp"
#include "../utils/GUIDGenerator.hpp"
#include "../models/Measurement.hpp"

// #include "WifiCommunicationService.hpp"

class MVPServer {
private:
    MeasurementRepository measurementRepository_;


public:
    MVPServer() {}

    // Starts a new analysis
    /*

    // Analyze the spring layers associated with an analysis
    Analysis analyseSpringLayers(const std::string& analysisId) {
        try {
            Analysis analysis = analysisRepo.retrieveAnalysisById(analysisId);
            // Here we assume that spring layer analysis involves something more complex, we just fetch the layers.
            auto springLayers = springLayerRepo.retrieveAllSpringLayersByAnalysisId(analysisId);
            for (const auto& layer : springLayers) {
                std::cout << "Analyzing Spring Layer: " << layer.getId() << std::endl;
            }
            analysisRepo.updateAnalysis(analysis);  // Update analysis after processing spring layers
            return analysis;
        } catch (const std::exception& e) {
            std::cerr << "Error analyzing spring layers: " << e.what() << std::endl;
            throw;
        }
    }  
    // Approve the spring layers for an analysis
    Analysis approveSpringLayers(const std::string& analysisId, const std::vector<std::string>& springLayerIds) {
        try {
            Analysis analysis = analysisRepo.retrieveAnalysisById(analysisId);
            for (const auto& layerId : springLayerIds) {
                SpringLayer layer = springLayerRepo.retrieveSpringLayerById(layerId);
                std::cout << "Approving Spring Layer: " << layer.getId() << std::endl;
                layer.setIsValid(true);
            }
            analysisRepo.updateAnalysis(analysis);  // Update analysis after approval
            return analysis;
        } catch (const std::exception& e) {
            std::cerr << "Error approving spring layers: " << e.what() << std::endl;
            throw;
        }
    }

    */


  void handleMeasurements(const std::string& measurements) {
    try {

        std::cout << "Measurements!!::" <<  measurements << std::endl;
        // Generate the grouping ID for this batch of measurements
        std::string groupingId = GUIDGenerator::generateGUID();

        // Parse the JSON string into a vector of Measurement objects
        std::vector<Measurement> parsedMeasurements = json_dto::from_json<std::vector<Measurement>>(measurements);

        // Iterate over the parsed measurements array and process each Measurement
        for (auto& measurement : parsedMeasurements) {

            std::cout << measurement.getOxygen() << std::endl;
            // Set the groupingId for the measurement
            measurement.setGroupingId(groupingId);

            // Write the Measurement object to the database (assuming db_ is available)
            measurementRepository_.write(measurement);
        }
    } catch (const std::exception& ex) {
        // Handle any exceptions that occur during parsing or database operations
        std::cerr << "Error parsing measurements: " << ex.what() << std::endl;
    }
}

std::vector<Measurement> getAllMeasurements()
{
    return measurementRepository_.read();
}



/*
    // Reject spring layers for an analysis
    Analysis rejectSpringLayers(const std::string& analysisId) {
        try {
            Analysis analysis = analysisRepo.retrieveAnalysisById(analysisId);
            // Process rejection logic
            std::cout << "Rejecting all spring layers for analysis." << std::endl;
            analysisRepo.updateAnalysis(analysis);  // Update analysis after rejection
            return analysis;
        } catch (const std::exception& e) {
            std::cerr << "Error rejecting spring layers: " << e.what() << std::endl;
            throw;
        }
    }

    */


    // Complete the analysis
   

  

};