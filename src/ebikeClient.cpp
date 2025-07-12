#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include "sensor/GPSSensor.h"
#include "comm/SocketServer.h"

// Hardware Abstraction Layer (HAL) simulation
class HAL {
private:
    int port;
    bool isAttached;
    
public:
    HAL() : port(-1), isAttached(false) {}
    
    bool attach(int portNumber) {
        if (isAttached) {
            std::cerr << "HAL already attached to port " << port << std::endl;
            return false;
        }
        port = portNumber;
        isAttached = true;
        std::cout << "HAL attached to port " << port << std::endl;
        return true;
    }
    
    bool release() {
        if (!isAttached) {
            std::cerr << "HAL not attached to any port" << std::endl;
            return false;
        }
        std::cout << "HAL released from port " << port << std::endl;
        port = -1;
        isAttached = false;
        return true;
    }
    
    bool isPortAttached() const {
        return isAttached;
    }
    
    int getPort() const {
        return port;
    }
};

class EBikeClient {
private:
    HAL hal;
    GPSSensor gpsSensor;
    std::string bikeId;
    std::ofstream dataFile;
    SocketClient socketClient;
    bool networkEnabled;
    
public:
    EBikeClient(const std::string& id) : bikeId(id), networkEnabled(false) {
        // Open data file for writing
        std::string filename = "data/sim-eBike-" + bikeId + ".csv";
        dataFile.open(filename);
        if (dataFile.is_open()) {
            dataFile << "timestamp,latitude,longitude,bike_id\n";
        }
        
        // Initialize network communication
        if (socketClient.initialize()) {
            networkEnabled = true;
            std::cout << "Network communication enabled for Bike " << bikeId << std::endl;
        } else {
            std::cout << "Network communication disabled for Bike " << bikeId << std::endl;
        }
    }
    
    ~EBikeClient() {
        if (dataFile.is_open()) {
            dataFile.close();
        }
    }
    
    bool initializeHAL(int port) {
        return hal.attach(port);
    }
    
    void releaseHAL() {
        hal.release();
    }
    
    void generateGPSReading() {
        gpsSensor.updateReading();
        
        // Print JSON format to console
        std::cout << "GPS Reading for Bike " << bikeId << ": " << gpsSensor.toJSON() << std::endl;
        
        // Send GPS data over network if enabled
        if (networkEnabled) {
            socketClient.sendDataMessage(bikeId, gpsSensor.toJSON());
        }
        
        // Write to CSV file
        if (dataFile.is_open()) {
            dataFile << gpsSensor.getTimestamp() << ","
                     << gpsSensor.getLatitude() << ","
                     << gpsSensor.getLongitude() << ","
                     << bikeId << "\n";
            dataFile.flush();
        }
    }
    
    void runSimulation(int duration, int interval) {
        std::cout << "Starting eBike simulation for Bike " << bikeId << std::endl;
        std::cout << "Duration: " << duration << " seconds, Interval: " << interval << " seconds" << std::endl;
        
        // Send join message to server
        if (networkEnabled) {
            socketClient.sendJoinMessage(bikeId);
        }
        
        auto startTime = std::chrono::steady_clock::now();
        auto endTime = startTime + std::chrono::seconds(duration);
        
        while (std::chrono::steady_clock::now() < endTime) {
            generateGPSReading();
            std::this_thread::sleep_for(std::chrono::seconds(interval));
        }
        
        std::cout << "Simulation completed for Bike " << bikeId << std::endl;
    }
    
    // Getter for testing
    bool isHALAttached() const {
        return hal.isPortAttached();
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <bike_id> [duration] [interval]" << std::endl;
        std::cerr << "Example: " << argv[0] << " 1 60 5" << std::endl;
        return 1;
    }
    
    std::string bikeId = argv[1];
    int duration = (argc > 2) ? std::stoi(argv[2]) : 60;  // Default 60 seconds
    int interval = (argc > 3) ? std::stoi(argv[3]) : 5;   // Default 5 seconds
    
    EBikeClient client(bikeId);
    
    // Initialize HAL on port 8080
    if (!client.initializeHAL(8080)) {
        std::cerr << "Failed to initialize HAL" << std::endl;
        return 1;
    }
    
    // Run simulation
    client.runSimulation(duration, interval);
    
    // Release HAL
    client.releaseHAL();
    
    return 0;
} 