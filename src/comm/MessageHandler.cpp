#include "MessageHandler.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

MessageHandler::MessageHandler() {
}

std::string MessageHandler::serializeMessage(const Message& msg) {
    std::ostringstream oss;
    oss << messageTypeToString(msg.type) << "|"
        << msg.bikeId << "|"
        << msg.payload << "|"
        << std::chrono::duration_cast<std::chrono::milliseconds>(
               msg.timestamp.time_since_epoch()).count();
    return oss.str();
}

Message MessageHandler::deserializeMessage(const std::string& data) {
    Message msg;
    std::istringstream iss(data);
    std::string typeStr, bikeId, payload, timestampStr;
    
    if (std::getline(iss, typeStr, '|') &&
        std::getline(iss, bikeId, '|') &&
        std::getline(iss, payload, '|') &&
        std::getline(iss, timestampStr, '|')) {
        
        msg.type = stringToMessageType(typeStr);
        msg.bikeId = bikeId;
        msg.payload = payload;
        
        try {
            auto timestamp = std::chrono::milliseconds(std::stoll(timestampStr));
            msg.timestamp = std::chrono::system_clock::time_point(timestamp);
        } catch (...) {
            msg.timestamp = std::chrono::system_clock::now();
        }
    }
    
    return msg;
}

Message MessageHandler::createJoinMessage(const std::string& bikeId) {
    return Message(MessageType::JOIN, bikeId, "JOIN_REQUEST");
}

Message MessageHandler::createJackMessage(const std::string& bikeId) {
    return Message(MessageType::JACK, bikeId, "JOIN_ACKNOWLEDGED");
}

Message MessageHandler::createDataMessage(const std::string& bikeId, const std::string& gpsData) {
    return Message(MessageType::DATA, bikeId, gpsData);
}

Message MessageHandler::createCommandMessage(const std::string& bikeId, const std::string& command) {
    return Message(MessageType::COMMAND, bikeId, command);
}

Message MessageHandler::createCommAckMessage(const std::string& bikeId, bool success) {
    std::string status = success ? "SUCCESS" : "FAILED";
    return Message(MessageType::COMMACK, bikeId, status);
}

Message MessageHandler::createSetupMessage(const std::string& bikeId, const std::string& config) {
    return Message(MessageType::SETUP, bikeId, config);
}

Message MessageHandler::createSetupAckMessage(const std::string& bikeId, bool success) {
    std::string status = success ? "SETUP_SUCCESS" : "SETUP_FAILED";
    return Message(MessageType::SETUP_ACK, bikeId, status);
}

std::string MessageHandler::processMessage(const Message& msg) {
    if (!isValidMessage(msg)) {
        return "INVALID_MESSAGE";
    }
    
    switch (msg.type) {
        case MessageType::JOIN:
            registerBike(msg.bikeId);
            return serializeMessage(createJackMessage(msg.bikeId));
            
        case MessageType::DATA:
            // Update bike activity timestamp
            activeBikes[msg.bikeId] = std::chrono::system_clock::now();
            return "DATA_RECEIVED";
            
        case MessageType::COMMAND:
            // Process command and return acknowledgment
            return serializeMessage(createCommAckMessage(msg.bikeId, true));
            
        case MessageType::SETUP:
            setBikeConfiguration(msg.bikeId, msg.payload);
            return serializeMessage(createSetupAckMessage(msg.bikeId, true));
            
        case MessageType::JACK:
        case MessageType::COMMACK:
        case MessageType::SETUP_ACK:
            // These are acknowledgment messages, just log them
            return "ACK_RECEIVED";
            
        default:
            return "UNKNOWN_MESSAGE_TYPE";
    }
}

bool MessageHandler::isValidMessage(const Message& msg) {
    if (msg.bikeId.empty()) {
        return false;
    }
    
    // Check if message is not too old (within 5 minutes)
    auto now = std::chrono::system_clock::now();
    auto age = std::chrono::duration_cast<std::chrono::minutes>(now - msg.timestamp);
    if (age.count() > 5) {
        return false;
    }
    
    return true;
}

void MessageHandler::registerBike(const std::string& bikeId) {
    activeBikes[bikeId] = std::chrono::system_clock::now();
    std::cout << "Bike " << bikeId << " registered" << std::endl;
}

void MessageHandler::unregisterBike(const std::string& bikeId) {
    activeBikes.erase(bikeId);
    std::cout << "Bike " << bikeId << " unregistered" << std::endl;
}

bool MessageHandler::isBikeActive(const std::string& bikeId) {
    auto it = activeBikes.find(bikeId);
    if (it == activeBikes.end()) {
        return false;
    }
    
    // Check if bike has been active in the last 2 minutes
    auto now = std::chrono::system_clock::now();
    auto lastSeen = std::chrono::duration_cast<std::chrono::minutes>(now - it->second);
    return lastSeen.count() < 2;
}

std::vector<std::string> MessageHandler::getActiveBikes() {
    std::vector<std::string> active;
    for (const auto& pair : activeBikes) {
        if (isBikeActive(pair.first)) {
            active.push_back(pair.first);
        }
    }
    return active;
}

void MessageHandler::setBikeConfiguration(const std::string& bikeId, const std::string& config) {
    bikeConfigurations[bikeId] = config;
    std::cout << "Configuration set for bike " << bikeId << std::endl;
}

std::string MessageHandler::getBikeConfiguration(const std::string& bikeId) {
    auto it = bikeConfigurations.find(bikeId);
    return (it != bikeConfigurations.end()) ? it->second : "";
}

std::string MessageHandler::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto tm = *std::localtime(&time_t);
    
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

MessageType MessageHandler::stringToMessageType(const std::string& typeStr) {
    if (typeStr == "JOIN") return MessageType::JOIN;
    if (typeStr == "JACK") return MessageType::JACK;
    if (typeStr == "DATA") return MessageType::DATA;
    if (typeStr == "COMMAND") return MessageType::COMMAND;
    if (typeStr == "COMMACK") return MessageType::COMMACK;
    if (typeStr == "SETUP") return MessageType::SETUP;
    if (typeStr == "SETUP_ACK") return MessageType::SETUP_ACK;
    return MessageType::DATA; // Default
}

std::string MessageHandler::messageTypeToString(MessageType type) {
    switch (type) {
        case MessageType::JOIN: return "JOIN";
        case MessageType::JACK: return "JACK";
        case MessageType::DATA: return "DATA";
        case MessageType::COMMAND: return "COMMAND";
        case MessageType::COMMACK: return "COMMACK";
        case MessageType::SETUP: return "SETUP";
        case MessageType::SETUP_ACK: return "SETUP_ACK";
        default: return "DATA";
    }
} 