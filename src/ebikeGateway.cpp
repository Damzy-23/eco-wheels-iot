#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include <map>
#include <iomanip>
#include "comm/SocketServer.h"

class EBikeGateway {
private:
    SocketServer server;
    std::map<std::string, std::string> bikeLocations;
    std::ofstream webDataFile;
    bool running;
    
public:
    EBikeGateway(int port = 8080) : server("0.0.0.0", port), running(false) {
        // Open web data file for map updates
        webDataFile.open("data/web_bike_data.json");
        if (webDataFile.is_open()) {
            webDataFile << "{\"bikes\":[]}" << std::endl;
            webDataFile.flush();
        }
        
        // Set up data callback
        server.setDataCallback([this](const std::string& bikeId, const std::string& gpsData) {
            this->updateBikeLocation(bikeId, gpsData);
        });
    }
    
    ~EBikeGateway() {
        stop();
        if (webDataFile.is_open()) {
            webDataFile.close();
        }
    }
    
    bool start() {
        if (!server.initialize()) {
            std::cerr << "Failed to initialize UDP server" << std::endl;
            return false;
        }
        
        server.start();
        running = true;
        
        std::cout << "eBike Gateway started on port " << server.getConnectedClients().size() << std::endl;
        std::cout << "Web interface data will be updated in data/web_bike_data.json" << std::endl;
        
        return true;
    }
    
    void stop() {
        if (running) {
            server.stop();
            running = false;
            std::cout << "eBike Gateway stopped" << std::endl;
        }
    }
    
    void updateBikeLocation(const std::string& bikeId, const std::string& gpsData) {
        bikeLocations[bikeId] = gpsData;
        std::cout << "Updated location for Bike " << bikeId << ": " << gpsData << std::endl;
        
        // Update web data file
        updateWebDataFile();
    }
    
    void updateWebDataFile() {
        if (!webDataFile.is_open()) return;
        
        // Clear file and write new data
        webDataFile.seekp(0);
        webDataFile.close();
        webDataFile.open("data/web_bike_data.json", std::ios::trunc);
        
        webDataFile << "{\n";
        webDataFile << "  \"timestamp\": \"" << getCurrentTimestamp() << "\",\n";
        webDataFile << "  \"active_bikes\": " << bikeLocations.size() << ",\n";
        webDataFile << "  \"bikes\": [\n";
        
        bool first = true;
        for (const auto& bike : bikeLocations) {
            if (!first) webDataFile << ",\n";
            webDataFile << "    {\n";
            webDataFile << "      \"id\": \"" << bike.first << "\",\n";
            webDataFile << "      \"data\": " << bike.second << "\n";
            webDataFile << "    }";
            first = false;
        }
        
        webDataFile << "\n  ]\n";
        webDataFile << "}" << std::endl;
        webDataFile.flush();
    }
    
    std::string getCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto tm = *std::localtime(&time_t);
        
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }
    
    void run() {
        if (!start()) {
            return;
        }
        
        std::cout << "Gateway running. Press Enter to stop..." << std::endl;
        std::cin.get();
        
        stop();
    }
    
    // Getter methods
    std::map<std::string, std::string> getBikeLocations() const {
        return bikeLocations;
    }
    
    std::vector<std::string> getActiveBikes() const {
        return server.getConnectedClients();
    }
    
    bool isRunning() const {
        return running;
    }
};

int main(int argc, char* argv[]) {
    int port = 8080;
    
    if (argc > 1) {
        port = std::stoi(argv[1]);
    }
    
    std::cout << "Starting Eco-Wheels eBike Gateway Server" << std::endl;
    std::cout << "Port: " << port << std::endl;
    std::cout << "Web interface: Open resources/html/map.html in your browser" << std::endl;
    std::cout << "Data file: data/web_bike_data.json" << std::endl;
    
    EBikeGateway gateway(port);
    gateway.run();
    
    return 0;
} 