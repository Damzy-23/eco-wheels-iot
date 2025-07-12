#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include <map>
#include <sstream>
#include "comm/SocketServer.h"

class ManagementClient {
private:
    SocketClient socketClient;
    std::string managerId;
    std::map<std::string, std::string> bikeStatuses;
    std::map<std::string, std::string> bikeConfigurations;
    
public:
    ManagementClient(const std::string& id = "MANAGER-001") : managerId(id) {
        if (!socketClient.initialize()) {
            std::cerr << "Failed to initialize management client" << std::endl;
        } else {
            std::cout << "Management Client initialized: " << managerId << std::endl;
        }
    }
    
    ~ManagementClient() {
        socketClient.disconnect();
    }
    
    // BASIC Protocol Commands
    bool sendJoinRequest(const std::string& bikeId) {
        std::cout << "Sending JOIN request to Bike " << bikeId << std::endl;
        return socketClient.sendJoinMessage(bikeId);
    }
    
    bool sendJackAcknowledgment(const std::string& bikeId) {
        std::cout << "Sending JACK acknowledgment to Bike " << bikeId << std::endl;
        // This would typically be a response to a JOIN message
        return true;
    }
    
    bool requestBikeData(const std::string& bikeId) {
        std::cout << "Requesting DATA from Bike " << bikeId << std::endl;
        // Send a command to request current GPS data
        std::string command = "{\"command\":\"GET_GPS_DATA\"}";
        return socketClient.sendCommandMessage(bikeId, command);
    }
    
    // ENHANCED Protocol Commands
    bool sendCommand(const std::string& bikeId, const std::string& command) {
        std::cout << "Sending COMMAND to Bike " << bikeId << ": " << command << std::endl;
        return socketClient.sendCommandMessage(bikeId, command);
    }
    
    bool sendSetupConfiguration(const std::string& bikeId, const std::string& config) {
        std::cout << "Sending SETUP configuration to Bike " << bikeId << std::endl;
        return socketClient.sendSetupMessage(bikeId, config);
    }
    
    // Maintenance Management Functions
    bool putBikeInMaintenance(const std::string& bikeId, const std::string& reason = "Scheduled maintenance") {
        std::ostringstream command;
        command << "{\"command\":\"MAINTENANCE_MODE\",\"reason\":\"" << reason << "\",\"duration\":3600}";
        return sendCommand(bikeId, command.str());
    }
    
    bool takeBikeOutOfMaintenance(const std::string& bikeId) {
        std::string command = "{\"command\":\"EXIT_MAINTENANCE_MODE\"}";
        return sendCommand(bikeId, command);
    }
    
    bool emergencyStop(const std::string& bikeId, const std::string& reason = "Emergency stop") {
        std::ostringstream command;
        command << "{\"command\":\"EMERGENCY_STOP\",\"reason\":\"" << reason << "\"}";
        return sendCommand(bikeId, command.str());
    }
    
    bool checkBikeStatus(const std::string& bikeId) {
        std::string command = "{\"command\":\"STATUS_CHECK\"}";
        return sendCommand(bikeId, command);
    }
    
    bool configureBike(const std::string& bikeId, int gpsInterval = 5, int batteryThreshold = 20) {
        std::ostringstream config;
        config << "{\"gps_update_interval\":" << gpsInterval 
               << ",\"battery_threshold\":" << batteryThreshold 
               << ",\"maintenance_interval\":7200}";
        return sendSetupConfiguration(bikeId, config.str());
    }
    
    // Interactive Management Interface
    void runInteractiveMode() {
        std::cout << "\n=== Eco-Wheels eBike Management System ===" << std::endl;
        std::cout << "Manager ID: " << managerId << std::endl;
        std::cout << "Available commands:" << std::endl;
        std::cout << "  1. join <bike_id>     - Send JOIN request" << std::endl;
        std::cout << "  2. data <bike_id>     - Request GPS data" << std::endl;
        std::cout << "  3. maintain <bike_id> [reason] - Put bike in maintenance" << std::endl;
        std::cout << "  4. release <bike_id>  - Take bike out of maintenance" << std::endl;
        std::cout << "  5. emergency <bike_id> [reason] - Emergency stop" << std::endl;
        std::cout << "  6. status <bike_id>   - Check bike status" << std::endl;
        std::cout << "  7. config <bike_id>   - Configure bike settings" << std::endl;
        std::cout << "  8. quit               - Exit management system" << std::endl;
        
        std::string command;
        while (true) {
            std::cout << "\nManagement> ";
            std::getline(std::cin, command);
            
            if (command == "quit" || command == "exit") {
                break;
            }
            
            processCommand(command);
        }
        
        std::cout << "Management system shutdown" << std::endl;
    }
    
private:
    void processCommand(const std::string& command) {
        std::istringstream iss(command);
        std::string cmd, bikeId, reason;
        
        iss >> cmd >> bikeId;
        
        if (cmd == "join") {
            if (bikeId.empty()) {
                std::cout << "Usage: join <bike_id>" << std::endl;
                return;
            }
            sendJoinRequest(bikeId);
        }
        else if (cmd == "data") {
            if (bikeId.empty()) {
                std::cout << "Usage: data <bike_id>" << std::endl;
                return;
            }
            requestBikeData(bikeId);
        }
        else if (cmd == "maintain") {
            if (bikeId.empty()) {
                std::cout << "Usage: maintain <bike_id> [reason]" << std::endl;
                return;
            }
            std::getline(iss, reason);
            if (!reason.empty()) {
                reason = reason.substr(1); // Remove leading space
            }
            putBikeInMaintenance(bikeId, reason.empty() ? "Scheduled maintenance" : reason);
        }
        else if (cmd == "release") {
            if (bikeId.empty()) {
                std::cout << "Usage: release <bike_id>" << std::endl;
                return;
            }
            takeBikeOutOfMaintenance(bikeId);
        }
        else if (cmd == "emergency") {
            if (bikeId.empty()) {
                std::cout << "Usage: emergency <bike_id> [reason]" << std::endl;
                return;
            }
            std::getline(iss, reason);
            if (!reason.empty()) {
                reason = reason.substr(1); // Remove leading space
            }
            emergencyStop(bikeId, reason.empty() ? "Emergency stop" : reason);
        }
        else if (cmd == "status") {
            if (bikeId.empty()) {
                std::cout << "Usage: status <bike_id>" << std::endl;
                return;
            }
            checkBikeStatus(bikeId);
        }
        else if (cmd == "config") {
            if (bikeId.empty()) {
                std::cout << "Usage: config <bike_id>" << std::endl;
                return;
            }
            configureBike(bikeId);
        }
        else if (!cmd.empty()) {
            std::cout << "Unknown command: " << cmd << std::endl;
            std::cout << "Type 'help' for available commands" << std::endl;
        }
    }
};

int main(int argc, char* argv[]) {
    std::string managerId = "MANAGER-001";
    
    if (argc > 1) {
        managerId = argv[1];
    }
    
    std::cout << "Starting Eco-Wheels eBike Management Client" << std::endl;
    std::cout << "Manager ID: " << managerId << std::endl;
    
    ManagementClient manager(managerId);
    
    if (argc > 2 && std::string(argv[2]) == "auto") {
        // Auto mode for testing
        std::cout << "Running in auto mode..." << std::endl;
        
        // Test basic protocol
        manager.sendJoinRequest("1");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        manager.requestBikeData("1");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        // Test enhanced protocol
        manager.putBikeInMaintenance("1", "Test maintenance");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        manager.takeBikeOutOfMaintenance("1");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        manager.configureBike("1", 3, 15);
        
        std::cout << "Auto mode completed" << std::endl;
    } else {
        // Interactive mode
        manager.runInteractiveMode();
    }
    
    return 0;
} 