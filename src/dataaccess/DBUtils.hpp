#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>

// DBUtils: A generic file-based database utility class
class DBUtils {
private:
    std::string dbFilePath;

public:
    explicit DBUtils(const std::string& filePath) : dbFilePath(filePath) {}

    void saveToFile(const std::vector<std::map<std::string, std::string>>& records) {
        std::ofstream file(dbFilePath, std::ios::trunc); // Open in truncate mode to overwrite
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open database file for writing.");
        }

        for (const auto& record : records) {
            for (const auto& [key, value] : record) {
                file << key << "=" << value << ";";
            }
            file << "\n"; // Newline separates records
        }

        file.close();
    }

    std::vector<std::map<std::string, std::string>> readFromFile() {
        std::vector<std::map<std::string, std::string>> records;
        std::ifstream file(dbFilePath);

        if (!file.is_open()) {
            throw std::runtime_error("Failed to open database file for reading.");
        }

        std::string line;
        while (std::getline(file, line)) {
            std::map<std::string, std::string> record;
            std::istringstream recordStream(line);
            std::string pair;

            while (std::getline(recordStream, pair, ';')) {
                size_t delimiterPos = pair.find('=');
                if (delimiterPos != std::string::npos) {
                    std::string key = pair.substr(0, delimiterPos);
                    std::string value = pair.substr(delimiterPos + 1);
                    record[key] = value;
                }
            }

            records.push_back(record);
        }

        file.close();
        return records;
    }
};

