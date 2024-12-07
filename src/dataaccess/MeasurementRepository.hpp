#pragma once 

#include "../models/Measurement.hpp"
#include "./SQLite.hpp"

#include <iostream>

class MeasurementRepository {

  public:

   void write(Measurement measurement) {
    
    SQLite& db = SQLite::getInstance("Measurement.db");


    // Prepare the SQL statement with the correct column names
   std::string sql =
    "INSERT INTO Measurements (temperature, oxygen, depth, groupingId, timestamp) "
    "VALUES ("
    + std::to_string(measurement.getTemperature()) + ", "
    + std::to_string(measurement.getOxygen()) + ", "
    + std::to_string(measurement.getDepth()) + ", '"
    + measurement.getGroupingId() + "', '"
    + measurement.getTimeStamp() + "');";


  
      if (db.executeInsertQuery(sql)) {
        std::cout << "Data inserted into Measurements!" << std::endl;
      }  

 
  }


  std::vector<Measurement> read() {

  SQLite& db = SQLite::getInstance("Measurement.db");

    std::vector<Measurement> measurements;
    const std::string query = "SELECT id, temperature, oxygen, depth, groupingId, timestamp FROM Measurements";

    // Use executeSelectQuery to fetch data into the vector
    if (!db.executeSelectQuery(query, MeasurementRepository::callback, &measurements)) {
        std::cerr << "Error executing select query." << std::endl;
    }

    return measurements;
}



private:

static int callback(void* data, int argc, char** argv, char** colNames) {
        std::vector<Measurement>* measurements = static_cast<std::vector<Measurement>*>(data);
        
        Measurement measurement;
        for (int i = 0; i < argc; i++) {
            if (std::string(colNames[i]) == "id") {
                measurement.setId(std::stoi(argv[i]));  // 'id' is an INTEGER
            } else if (std::string(colNames[i]) == "temperature") {
                measurement.setTemperature(std::stod(argv[i]));  // 'temperature' is a REAL
            } else if (std::string(colNames[i]) == "oxygen") {
                measurement.setOxygen(std::stod(argv[i]));  // 'oxygen' is a REAL
            } else if (std::string(colNames[i]) == "depth") {
                measurement.setDepth(std::stod(argv[i]));  // 'depth' is a REAL
            } else if (std::string(colNames[i]) == "groupingId") {
                measurement.setGroupingId(argv[i] ? argv[i] : "");  // 'groupingId' is TEXT
            } else if (std::string(colNames[i]) == "timestamp") {
                measurement.setTimeStamp(argv[i] ? argv[i] : "");  // 'timestamp' is TEXT
            }
        }
        
        measurements->push_back(measurement);
        return 0;  // Return 0 to continue processing the result
    }



};