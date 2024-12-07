#pragma once
#include "DBUtils.hpp"

class SpringLayer {
public:
    std::string id;
    std::string analysisId;
    std::string data;

    SpringLayer(const std::string& id, const std::string& analysisId, const std::string& data)
        : id(id), analysisId(analysisId), data(data) {}

    friend std::ostream& operator<<(std::ostream& os, const SpringLayer& layer) {
        os << "SpringLayer { id: " << layer.id << ", analysisId: " << layer.analysisId
           << ", data: " << layer.data << " }";
        return os;
    }
};

// SpringLayerRepository class
class SpringLayerRepository {
private:
    DBUtils dbUtils;

    std::vector<SpringLayer> deserialize(const std::vector<std::map<std::string, std::string>>& records) {
        std::vector<SpringLayer> layers;

        for (const auto& record : records) {
            if (record.find("id") != record.end() && record.find("analysisId") != record.end() &&
                record.find("data") != record.end()) {
                layers.emplace_back(record.at("id"), record.at("analysisId"), record.at("data"));
            }
        }

        return layers;
    }

    std::vector<std::map<std::string, std::string>> serialize(const std::vector<SpringLayer>& layers) {
        std::vector<std::map<std::string, std::string>> records;

        for (const auto& layer : layers) {
            records.push_back({{"id", layer.id}, {"analysisId", layer.analysisId}, {"data", layer.data}});
        }

        return records;
    }

public:
    explicit SpringLayerRepository(const std::string& filePath) : dbUtils(filePath) {}

    void saveSpringLayer(const SpringLayer& springLayer) {
        auto layers = retrieveAllSpringLayers();
        layers.push_back(springLayer);
        dbUtils.saveToFile(serialize(layers));
    }

    SpringLayer retrieveSpringLayerById(const std::string& id) {
        auto layers = retrieveAllSpringLayers();

        for (const auto& layer : layers) {
            if (layer.id == id) {
                return layer;
            }
        }

        throw std::runtime_error("SpringLayer not found with id: " + id);
    }

    std::vector<SpringLayer> retrieveAllSpringLayersByAnalysisId(const std::string& analysisId) {
        auto layers = retrieveAllSpringLayers();
        std::vector<SpringLayer> result;

        for (const auto& layer : layers) {
            if (layer.analysisId == analysisId) {
                result.push_back(layer);
            }
        }

        return result;
    }

    void updateSpringLayer(const SpringLayer& springLayer) {
        auto layers = retrieveAllSpringLayers();
        bool updated = false;

        for (auto& layer : layers) {
            if (layer.id == springLayer.id) {
                layer = springLayer;
                updated = true;
                break;
            }
        }

        if (!updated) {
            throw std::runtime_error("SpringLayer not found with id: " + springLayer.id);
        }

        dbUtils.saveToFile(serialize(layers));
    }

    std::vector<SpringLayer> retrieveAllSpringLayers() {
        return deserialize(dbUtils.readFromFile());
    }
};
