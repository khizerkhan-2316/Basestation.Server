
#pragma once

#include <chrono>
#include <iomanip>
#include <iostream>
#include <vector>
#include <sstream>
#include <ctime>
#include <json_dto/pub.hpp>  // Assuming you're using json_dto for deserialization

class MeasurementDepth {
    public:
        std::string measurementDepth;  // We expect this to be a string




      MeasurementDepth(){}

      MeasurementDepth(std::string depth)
      {
        measurementDepth = depth;
      }

      void setMeasurementDepth(std::string depth)
      {
        measurementDepth = depth;
      }

      std::string getMeasurementDepth()
      {
        return measurementDepth;
      }

        // The json_dto macros for serializing/deserializing JSON
        template <typename JSON_IO>
        void json_io(JSON_IO& io) {
                io & json_dto::mandatory("measurementDepth", measurementDepth);

      
        }

       
      };