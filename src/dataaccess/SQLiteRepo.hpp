
#include <sqlite3.h>

#include <iostream>

class SQLite {
  static void write(Measurement measurement) {
    sqlite3* db;
    sqlite3_open("Measurement.db", &db);  // Open the database

    // Prepare the SQL statement with the correct column names
    std::string sql =
        "INSERT INTO Measurements (temprature, oxygen, depth, groupingId) "
        "VALUES "
        "(" +
        std::to_string(measurement.temprature) + ", " +
        std::to_string(measurement.oxygen) + ", " + std::to_string(depth) +
        ", " + std::to_string(groupingId) + "," + std::to_string(timestamp) +
        ");";

    // Execute the query
    sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr);

    // Close the database
    sqlite3_close(db);
  }

  static void read() {}
}