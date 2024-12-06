#include "../models/Measurement.hpp"
#include <sqlite3.h>


#include <iostream>

class SQLite {
  public:

   void write(Measurement measurement) {
    sqlite3* db;
    sqlite3_open("Measurement.db", &db);  // Open the database

    // Prepare the SQL statement with the correct column names
    std::string sql =
        "INSERT INTO Measurements (temperature, oxygen, depth, groupingId, timestamp) "
        "VALUES "
        "(" +
        std::to_string(measurement.getTemperature()) + ", " +
        std::to_string(measurement.getOxygen()) + ", " + std::to_string(measurement.getDepth()) +
        ", " + measurement.getGroupingId() + "," + measurement.getTimeStamp() +
        ");";

    // Execute the query
    sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr);

    // Close the database
    sqlite3_close(db);
  }

  static void read() {}
};