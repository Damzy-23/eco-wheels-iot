#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <chrono>

// Message types for the communication protocol
enum class MessageType {
    JOIN,       // Client joining the network
    JACK,       // Client acknowledgment
    DATA,       // GPS data transmission
    COMMAND,    // Management command
    COMMACK,    // Command acknowledgment
    SETUP,      // Configuration setup
    SETUP_ACK   // Setup acknowledgment
};

// Message structure
struct Message {
    MessageType type;
    std::string bikeId;
    std::string payload;
    std::chrono::system_clock::time_point timestamp;
    
    Message() : type(MessageType::DATA), bikeId(""), payload(""), 
                timestamp(std::chrono::system_clock::now()) {}
    
    Message(MessageType t, const std::string& id, const std::string& p) 
        : type(t), bikeId(id), payload(p), timestamp(std::chrono::system_clock::now()) {}
};

class MessageHandler {
private:
    std::map<std::string, std::chrono::system_clock::time_point> activeBikes;
    std::map<std::string, std::string> bikeConfigurations;
    
public:
    MessageHandler();
    
    // Message serialization/deserialization
    std::string serializeMessage(const Message& msg);
    Message deserializeMessage(const std::string& data);
    
    // Message creation helpers
    Message createJoinMessage(const std::string& bikeId);
    Message createJackMessage(const std::string& bikeId);
    Message createDataMessage(const std::string& bikeId, const std::string& gpsData);
    Message createCommandMessage(const std::string& bikeId, const std::string& command);
    Message createCommAckMessage(const std::string& bikeId, bool success);
    Message createSetupMessage(const std::string& bikeId, const std::string& config);
    Message createSetupAckMessage(const std::string& bikeId, bool success);
    
    // Message processing
    std::string processMessage(const Message& msg);
    bool isValidMessage(const Message& msg);
    
    // Bike management
    void registerBike(const std::string& bikeId);
    void unregisterBike(const std::string& bikeId);
    bool isBikeActive(const std::string& bikeId);
    std::vector<std::string> getActiveBikes();
    
    // Configuration management
    void setBikeConfiguration(const std::string& bikeId, const std::string& config);
    std::string getBikeConfiguration(const std::string& bikeId);
    
    // Utility functions
    std::string getCurrentTimestamp();
    MessageType stringToMessageType(const std::string& typeStr);
    std::string messageTypeToString(MessageType type);
};

#endif // MESSAGE_HANDLER_H 