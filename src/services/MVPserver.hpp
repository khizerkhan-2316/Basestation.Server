#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <restinio/all.hpp>
#include <./dataaccess/AnalysisRepo.hpp>
#include <./dataaccess/MeasurementRepo.hpp>
#include <./dataaccess/SpringLayerRepo.hpp>

// #include "WifiCommunicationService.hpp"

class MVPServer {
private:
    AnalysisRepository analysisRepo;
    MeasurementRepository measurementRepo;
    SpringLayerRepository springLayerRepo;

public:
    MVPServer(AnalysisRepository& analysisRepository, MeasurementRepository& measurementRepository, SpringLayerRepository& springLayerRepository)
        : analysisRepo(analysisRepository), measurementRepo(measurementRepository), springLayerRepo(springLayerRepository) {}

    // Starts a new analysis
    void startAnalysis() {
        Analysis newAnalysis;
        newAnalysis.addMeasurement(25.0f, 21.0f, 1.0f); // Dummy data, assume analysis starts with a default measurement
        analysisRepo.saveAnalysis(newAnalysis);
        std::cout << "Analysis started." << std::endl;
    }

    // Add a measurement to an existing analysis
    void addMeasurement(const std::string& analysisId, const MeasurementData& measurement) {
        try {
            Analysis analysis = analysisRepo.retrieveAnalysisById(analysisId);
            analysis.addMeasurement(measurement.getTemperature(), measurement.getOxygen(), measurement.getPressure());
            analysisRepo.updateAnalysis(analysis);
            std::cout << "Measurement added to analysis." << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error adding measurement: " << e.what() << std::endl;
        }
    }

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

    // Complete the analysis
    Analysis completeAnalysis(const std::string& analysisId) {
        try {
            Analysis analysis = analysisRepo.retrieveAnalysisById(analysisId);
            analysis.complete();  // Assuming there's a method that sets the status to completed
            analysisRepo.updateAnalysis(analysis);
            std::cout << "Analysis completed." << std::endl;
            return analysis;
        } catch (const std::exception& e) {
            std::cerr << "Error completing analysis: " << e.what() << std::endl;
            throw;
        }
    }

    // Retrieve an analysis by its ID
    Analysis getAnalysisById(const std::string& id) {
        try {
            return analysisRepo.retrieveAnalysisById(id);
        } catch (const std::exception& e) {
            std::cerr << "Error retrieving analysis: " << e.what() << std::endl;
            throw;
        }
    }

    // Get all analyses
    std::vector<Analysis> getAllAnalysis() {
        try {
            return analysisRepo.getAllAnalysis();
        } catch (const std::exception& e) {
            std::cerr << "Error retrieving all analyses: " << e.what() << std::endl;
            throw;
        }
    }

    // Get the status of an analysis
    std::string getAnalysisStatus(const std::string& analysisId) {
        try {
            Analysis analysis = analysisRepo.retrieveAnalysisById(analysisId);
            return (analysis.getStatus() == AnalysisStatus::Completed) ? "Completed" : "In Progress";
        } catch (const std::exception& e) {
            std::cerr << "Error getting analysis status: " << e.what() << std::endl;
            throw;
        }
    }

    // Check if the submarine is available (mock implementation)
    bool isSubmarineAvailable() {
        // For simplicity, assume it's always available
        return true;
    }
};