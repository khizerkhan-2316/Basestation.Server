#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
class SpringLayer {
 private:
  std::string id;
  float layerDepth;
  std::chrono::system_clock::time_point timestamp;
  bool isSpringLayerValid;

  template <typename Json_Io>
  void json_io(Json_Io& io) {
    io& json_dto::mandatory("id", id) &
        json_dto::mandatory("layer Depth", layerDepth) &
        json_dto::mandatory("timestamp", timestamp) &
        json_dto::mandatory("valid spring layer", isSpringLayerValid);
  }

 public:
  // Constructor to initialize SpringLayer with layer depth
  SpringLayer(float layerDepth)
      : layerDepth(layerDepth), isSpringLayerValid(true) {
    // Generate a unique ID and set the current timestamp
    id = generateUniqueId();
    timestamp = std::chrono::system_clock::now();
  }
  // Method to generate a unique ID (for simplicity, using current time as a
  // basis)
  static std::string generateUniqueId() {
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                      now.time_since_epoch())
                      .count();
    return "SL" + std::to_string(now_ms);
  }
  // Getter for layer ID
  std::string getId() const { return id; }
  // Setter for layer ID
  void setId(const std::string& newId) { id = newId; }

  // Getter for layer depth
  float getLayerDepth() const { return layerDepth; }
  // Getter for isSpringLayerValid
  bool getIsSpringLayerValid() const { return isSpringLayerValid; }
  // Setter for isSpringLayerValid
  void setIsSpringLayerValid(bool isValid) { isSpringLayerValid = isValid; }
  // Method to get formatted timestamp as a string
  std::string getFormattedTimestamp() const {
    auto timeT = std::chrono::system_clock::to_time_t(timestamp);
    std::tm* tmPtr = std::localtime(&timeT);
    std::ostringstream oss;
    oss << std::put_time(tmPtr, "%Y-%m-%d %H:%M:%S");
    return oss.str();
  }
  // Method to display the details of the SpringLayer
  void display() const {
    std::cout << "Spring Layer ID: " << id << std::endl;
    std::cout << "Layer Depth: " << layerDepth << " meters" << std::endl;
    std::cout << "Timestamp: " << getFormattedTimestamp() << std::endl;
    std::cout << "Layer Valid: " << (isSpringLayerValid ? "Yes" : "No")
              << std::endl;
  }

  // Method to update the layer depth
  void updateLayerDepth(float newDepth) { layerDepth = newDepth; }
  // Method to update the timestamp
  void updateTimestamp() { timestamp = std::chrono::system_clock::now(); }
  // Method to validate the SpringLayer
  void validateLayer() { isSpringLayerValid = true; }
  // Method to invalidate the SpringLayer
  void invalidateLayer() { isSpringLayerValid = false; }

  // Method to serialize the SpringLayer data
  std::string serialize() const {
    std::ostringstream oss;
    oss << id << "," << layerDepth << "," << getFormattedTimestamp() << ","
        << (isSpringLayerValid ? "1" : "0");
    return oss.str();
  }
};