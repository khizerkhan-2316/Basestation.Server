// models/Submarine.h
#ifndef SUBMARINE_H
#define SUBMARINE_H

#include <string>
#include <json_dto/pub.hpp>

class Submarine {
public:
    std::string id;
    std::string name;
    bool isAvailable;
    float length;
    float width;
    float weight;

    // Constructor
    Submarine(std::string id = "", std::string name = "", bool isAvailable = true,
              float length = 0.0f, float width = 0.0f, float weight = 0.0f)
        : id(id), name(name), isAvailable(isAvailable), length(length), width(width), weight(weight) {}

    // JSON serialization/deserialization using <json_dto>
    template <typename JSON_IO>
    void json_io(JSON_IO &io) {
        io &json_dto::mandatory("id", id)
           &json_dto::mandatory("name", name)
           &json_dto::mandatory("isAvailable", isAvailable)
           &json_dto::mandatory("length", length)
           &json_dto::mandatory("width", width)
           &json_dto::mandatory("weight", weight);
    }
};

#endif // SUBMARINE_H
