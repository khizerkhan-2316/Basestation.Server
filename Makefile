# Variables
CXX = g++
CXXFLAGS = -std=c++17 -Isrc/../ngk/restinio/dev/restinio -Isrc/controllers -Isrc/models -Isrc/data -I/usr/include
LIBS = -L/path/to/restinio/lib -L/usr/local/lib -lrestinio -lboost_system -pthread
SRC_DIR = src
EXEC = $(SRC_DIR)/server  # Change here to place the executable in the src directory

# Directories for the server
CONTROLLERS_DIR = $(SRC_DIR)/controllers
MODELS_DIR = $(SRC_DIR)/models
DATA_DIR = $(SRC_DIR)/data

# Find all .cpp files in src folder
SOURCES = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(CONTROLLERS_DIR)/*.cpp) $(wildcard $(MODELS_DIR)/*.cpp) $(wildcard $(DATA_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o) $(SOURCES:$(CONTROLLERS_DIR)/%.cpp=$(BUILD_DIR)/%.o) $(SOURCES:$(MODELS_DIR)/%.cpp=$(BUILD_DIR)/%.o) $(SOURCES:$(DATA_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Build target: server executable
$(EXEC): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(EXEC) $(LIBS)

# Clean the build directory (remove .o files and executable)
clean:
	rm -f $(OBJECTS) $(EXEC)

# Run the server (optional, you can execute it with `make run`)
run: $(EXEC)
	./$(EXEC)

# Install the build (optional step to move files to correct location)
install: $(EXEC)
	mv $(BUILD_DIR)/* /var/www/html

# Serve static files (React build in this example)
# Make sure you have the React build files in src/build/ and want them served at /var/www/html
serve: $(EXEC)
	@echo "Serving static files..."
	@sudo cp -r $(BUILD_DIR)/* /var/www/html
