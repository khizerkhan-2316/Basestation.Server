#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <restinio/all.hpp>
#include "../dataaccess/MeasurementRepository.hpp"
#include "../utils/GUIDGenerator.hpp"
#include "../models/Measurement.hpp"
#include <cstdint>
#include <sstream>   

// #include "WifiCommunicationService.hpp"

class MVPServer {
private:
    MeasurementRepository measurementRepository_;


public:
    MVPServer() {}

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

bool isHardwareTestValid(uint8_t byte)
{

        std::cout << "BYTE INSIDE ISHARDWARETEST VALID:" << byte << std::endl;

    if (!byte)
    {
            return true;

    } else
    {

        return false;
    }
    if (!byte) return "OK. Ingen fejl.";
    std::stringstream ss;
    const char* errormsg [5] = {"Temperature sensor", 
                                "Depth Sensor", 
                                "Oxygen sensor",
                                "Actuator water take-in",
                                "Actuator water push-out"};
    for(int i=0; i<5; i++)
    if(byte & (1<<i))
    {
        return false;
    }

    return true;
}

};