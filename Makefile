# Makefile for Eco-Wheels IoT e-Bike Monitoring System

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
INCLUDES = -Isrc -Isrc/sensor -Isrc/comm
LDFLAGS = -lws2_32

# Directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
TEST_DIR = tests

# Source files
GPS_SENSOR_SRC = $(SRC_DIR)/sensor/GPSSensor.cpp
MESSAGE_HANDLER_SRC = $(SRC_DIR)/comm/MessageHandler.cpp
SOCKET_SERVER_SRC = $(SRC_DIR)/comm/SocketServer.cpp
EBIKE_CLIENT_SRC = $(SRC_DIR)/ebikeClient.cpp
EBIKE_GATEWAY_SRC = $(SRC_DIR)/ebikeGateway.cpp
MANAGEMENT_CLIENT_SRC = $(SRC_DIR)/managementClient.cpp

# Test files
GPS_SENSOR_TEST = $(TEST_DIR)/GPSSensorTest.cpp
EBIKE_CLIENT_TEST = $(TEST_DIR)/ebikeClientTest.cpp
MISC_UTILS_TEST = $(TEST_DIR)/MiscUtilsTest.cpp

# Object files
GPS_SENSOR_OBJ = $(BUILD_DIR)/GPSSensor.o
MESSAGE_HANDLER_OBJ = $(BUILD_DIR)/MessageHandler.o
SOCKET_SERVER_OBJ = $(BUILD_DIR)/SocketServer.o
EBIKE_CLIENT_OBJ = $(BUILD_DIR)/ebikeClient.o
EBIKE_GATEWAY_OBJ = $(BUILD_DIR)/ebikeGateway.o
MANAGEMENT_CLIENT_OBJ = $(BUILD_DIR)/managementClient.o

# Executables
EBIKE_CLIENT_EXE = $(BIN_DIR)/ebikeClient
EBIKE_GATEWAY_EXE = $(BIN_DIR)/ebikeGateway
MANAGEMENT_CLIENT_EXE = $(BIN_DIR)/managementClient

# Test executables
GPS_SENSOR_TEST_EXE = $(BIN_DIR)/GPSSensorTest
EBIKE_CLIENT_TEST_EXE = $(BIN_DIR)/ebikeClientTest
MISC_UTILS_TEST_EXE = $(BIN_DIR)/MiscUtilsTest

# Default target
all: $(EBIKE_CLIENT_EXE) $(EBIKE_GATEWAY_EXE) $(MANAGEMENT_CLIENT_EXE)

# Create build directories
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Main application targets
$(EBIKE_CLIENT_EXE): $(EBIKE_CLIENT_OBJ) $(GPS_SENSOR_OBJ) $(MESSAGE_HANDLER_OBJ) $(SOCKET_SERVER_OBJ) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(EBIKE_GATEWAY_EXE): $(EBIKE_GATEWAY_OBJ) $(MESSAGE_HANDLER_OBJ) $(SOCKET_SERVER_OBJ) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(MANAGEMENT_CLIENT_EXE): $(MANAGEMENT_CLIENT_OBJ) $(MESSAGE_HANDLER_OBJ) $(SOCKET_SERVER_OBJ) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Object file targets
$(GPS_SENSOR_OBJ): $(GPS_SENSOR_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(MESSAGE_HANDLER_OBJ): $(MESSAGE_HANDLER_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(SOCKET_SERVER_OBJ): $(SOCKET_SERVER_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(EBIKE_CLIENT_OBJ): $(EBIKE_CLIENT_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(EBIKE_GATEWAY_OBJ): $(EBIKE_GATEWAY_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(MANAGEMENT_CLIENT_OBJ): $(MANAGEMENT_CLIENT_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Test targets
test: test-gps test-ebike test-misc

test-gps: $(GPS_SENSOR_TEST_EXE)
	./$(GPS_SENSOR_TEST_EXE)

test-ebike: $(EBIKE_CLIENT_TEST_EXE)
	./$(EBIKE_CLIENT_TEST_EXE)

test-misc: $(MISC_UTILS_TEST_EXE)
	./$(MISC_UTILS_TEST_EXE)

# Test executables
$(GPS_SENSOR_TEST_EXE): $(GPS_SENSOR_TEST) $(GPS_SENSOR_OBJ) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ -lcatch2

$(EBIKE_CLIENT_TEST_EXE): $(EBIKE_CLIENT_TEST) $(EBIKE_CLIENT_OBJ) $(GPS_SENSOR_OBJ) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ -lcatch2

$(MISC_UTILS_TEST_EXE): $(MISC_UTILS_TEST) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $< -lcatch2

# Utility targets
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

run-ebike-1: $(EBIKE_CLIENT_EXE)
	./$(EBIKE_CLIENT_EXE) 1 30 3

run-ebike-2: $(EBIKE_CLIENT_EXE)
	./$(EBIKE_CLIENT_EXE) 2 30 3

run-ebike-3: $(EBIKE_CLIENT_EXE)
	./$(EBIKE_CLIENT_EXE) 3 30 3

run-ebike-4: $(EBIKE_CLIENT_EXE)
	./$(EBIKE_CLIENT_EXE) 4 30 3

run-gateway: $(EBIKE_GATEWAY_EXE)
	./$(EBIKE_GATEWAY_EXE) 8080

run-management: $(MANAGEMENT_CLIENT_EXE)
	./$(MANAGEMENT_CLIENT_EXE)

# Documentation
docs:
	@echo "Building documentation..."
	@echo "See README.md and WS_1_README.md for project documentation"

# Help target
help:
	@echo "Available targets:"
	@echo "  all              - Build all main applications"
	@echo "  test             - Run all tests"
	@echo "  test-gps         - Run GPS sensor tests"
	@echo "  test-ebike       - Run eBike client tests"
	@echo "  test-misc        - Run misc utils tests"
	@echo "  run-ebike-1      - Run eBike client 1 simulation"
	@echo "  run-ebike-2      - Run eBike client 2 simulation"
	@echo "  run-ebike-3      - Run eBike client 3 simulation"
	@echo "  run-ebike-4      - Run eBike client 4 simulation"
	@echo "  run-gateway      - Run eBike gateway server"
	@echo "  run-management   - Run management client"
	@echo "  clean            - Remove build artifacts"
	@echo "  help             - Show this help message"

.PHONY: all test test-gps test-ebike test-misc clean docs help run-ebike-1 run-ebike-2 run-ebike-3 run-ebike-4 run-gateway run-management 