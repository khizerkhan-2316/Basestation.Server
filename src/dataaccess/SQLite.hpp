#pragma once

#ifndef SQLite_HPP
#define SQLite_HPP
#include <iostream>

#include <sqlite3.h>
#include <string>
#include <vector>

class SQLite
{
public:
  static SQLite &getInstance(const std::string &dbName = "Measurement")
  {
    static SQLite instance(dbName); // Instance is created only once
    return instance;
  }

  bool initializeScript() {
    const std::string createTableQuery = R"(
        CREATE TABLE IF NOT EXISTS Measurements (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            groupingId TEXT,
            temperature REAL,
            oxygen REAL,
            depth REAL,
            timestamp TEXT
        );
    )";

    return executeInsertQuery(createTableQuery);
}


SQLite(const std::string& dbName) : db(nullptr), dbName(dbName) {}  
~SQLite() { closeConnection(); }
  // Function for executing INSERT queries
  bool executeInsertQuery(const std::string &query)
  {

    // Open the connection for this query
    if (!openConnection())
    {
      return false;
    }

    // Execute the query
    if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, nullptr) !=
        SQLITE_OK)
    {
    const char* errorMessage = sqlite3_errmsg(db);

    // Print a more detailed error message
    std::cerr << "ERROR WRITING to SQL: " << errorMessage << std::endl;

      closeConnection(); // Close the connection after query execution
      return false;
    }

    std::cout << "INSERT query executed successfully!" << std::endl;
    closeConnection(); // Close the connection after query execution
    return true;
  }

  // Function for executing SELECT queries and fetching results
 bool executeSelectQuery(const std::string& query,
                        int (*callback)(void*, int, char**, char**),
                        void* data) {

    // Open the connection for this query
    if (!openConnection()) {
        return false;
    }

    // Execute the query and call the callback for each row
    if (sqlite3_exec(db, query.c_str(), callback, data, nullptr) != SQLITE_OK) {
        std::cout << "ERROR reading from sql: " << sqlite3_errmsg(db) << std::endl;
        closeConnection();  // Close the connection after query execution
        return false;
    }

    closeConnection();  // Close the connection after query execution
    return true;
}


private:
  sqlite3    *db;     // SQLite database object
  std::string dbName; // Database name
  bool        openConnection()
  {
    if (sqlite3_open(dbName.c_str(), &db) != SQLITE_OK)
    {
      std::cerr << "Error opening database: " << sqlite3_errmsg(db)
                << std::endl;
      return false;
    }
    return true;
  }
  void closeConnection()
  {
    if (db)
    {
      sqlite3_close(db);
    }
  }
};

#endif
