#include <iostream>
#include <vector>
#include <string>
#include <future>
#include "DBUtils.hpp"
#include <./models/Submarine.hpp>

class SubmarineRepository {
private:
    DBUtils dbUtils;

    // Deserialize a map back to a Submarine object
    Submarine deserialize(const std::map<std::string, std::string>& record) {
        Submarine submarine;
        submarine.id = record.at("id");
        submarine.name = record.at("name");
        submarine.isAvailable = record.at("isAvailable") == "true";
        submarine.length = std::stof(record.at("length"));
        submarine.width = std::stof(record.at("width"));
        submarine.weight = std::stof(record.at("weight"));
        return submarine;
    }

public:
    explicit SubmarineRepository(const std::string& filePath) : dbUtils(filePath) {}

    // Retrieve all submarines asynchronously
    std::future<std::vector<Submarine>> RetrieveAllSubmarines() {
        return std::async(std::launch::async, [this]() {
            std::vector<Submarine> submarines;
            auto records = dbUtils.readFromFile();

            for (const auto& record : records) {
                submarines.push_back(deserialize(record));
            }

            return submarines;
        });
    }

    // Retrieve a submarine by ID asynchronously
    std::future<Submarine> RetrieveSubmarineById(const std::string& id) {
        return std::async(std::launch::async, [this, id]() {
            auto submarines = dbUtils.readFromFile();
            for (const auto& record : submarines) {
                if (record.at("id") == id) {
                    return deserialize(record);
                }
            }
            throw std::runtime_error("Submarine with ID " + id + " not found.");
        });
    }
};
