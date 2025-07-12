#include "SocketServer.h"
#include <iostream>
#include <sstream>
#include <algorithm>

// SocketServer Implementation
SocketServer::SocketServer(const std::string& host, int port) 
    : serverSocket(INVALID_SOCKET), host(host), port(port), running(false) {
}

SocketServer::~SocketServer() {
    stop();
    cleanupWinsock();
}

void SocketServer::initializeWinsock() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
    }
#endif
}

void SocketServer::cleanupWinsock() {
#ifdef _WIN32
    WSACleanup();
#endif
}

bool SocketServer::initialize() {
    initializeWinsock();
    
    serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket" << std::endl;
        return false;
    }
    
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(host.c_str());
    
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Failed to bind socket to port " << port << std::endl;
        return false;
    }
    
    std::cout << "UDP Server initialized on " << host << ":" << port << std::endl;
    return true;
}

void SocketServer::start() {
    if (!running) {
        running = true;
        listenerThread = std::thread(&SocketServer::listenerLoop, this);
        std::cout << "UDP Server started" << std::endl;
    }
}

void SocketServer::stop() {
    if (running) {
        running = false;
        if (listenerThread.joinable()) {
            listenerThread.join();
        }
        
        if (serverSocket != INVALID_SOCKET) {
#ifdef _WIN32
            closesocket(serverSocket);
#else
            close(serverSocket);
#endif
            serverSocket = INVALID_SOCKET;
        }
        std::cout << "UDP Server stopped" << std::endl;
    }
}

bool SocketServer::isRunning() const {
    return running;
}

void SocketServer::listenerLoop() {
    char buffer[1024];
    struct sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    
    while (running) {
        int bytesReceived = recvfrom(serverSocket, buffer, sizeof(buffer) - 1, 0,
                                   (struct sockaddr*)&clientAddr, &clientAddrLen);
        
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            std::string message(buffer);
            handleClientMessage(message, clientAddr);
        }
    }
}

void SocketServer::handleClientMessage(const std::string& data, const struct sockaddr_in& clientAddr) {
    Message msg = messageHandler.deserializeMessage(data);
    
    if (msg.type == MessageType::JOIN) {
        addClient(msg.bikeId, clientAddr);
    }
    
    std::string response = messageHandler.processMessage(msg);
    
    if (msg.type == MessageType::DATA) {
        bikeData.push_back(msg.payload);
        if (dataCallback) {
            dataCallback(msg.bikeId, msg.payload);
        }
    }
    
    if (!response.empty() && response != "DATA_RECEIVED") {
        sendUDP(response, clientAddr);
    }
}

bool SocketServer::sendUDP(const std::string& message, const struct sockaddr_in& clientAddr) {
    int bytesSent = sendto(serverSocket, message.c_str(), message.length(), 0,
                          (struct sockaddr*)&clientAddr, sizeof(clientAddr));
    return bytesSent > 0;
}

void SocketServer::addClient(const std::string& bikeId, const struct sockaddr_in& clientAddr) {
    clients[bikeId] = clientAddr;
    std::cout << "Client " << bikeId << " connected from " 
              << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;
}

void SocketServer::removeClient(const std::string& bikeId) {
    clients.erase(bikeId);
    std::cout << "Client " << bikeId << " disconnected" << std::endl;
}

std::vector<std::string> SocketServer::getConnectedClients() const {
    std::vector<std::string> clientList;
    for (const auto& client : clients) {
        clientList.push_back(client.first);
    }
    return clientList;
}

void SocketServer::setDataCallback(std::function<void(const std::string&, const std::string&)> callback) {
    dataCallback = callback;
}

void SocketServer::broadcastMessage(const std::string& message) {
    for (const auto& client : clients) {
        sendUDP(message, client.second);
    }
}

void SocketServer::sendToClient(const std::string& bikeId, const std::string& message) {
    auto it = clients.find(bikeId);
    if (it != clients.end()) {
        sendUDP(message, it->second);
    }
}

std::string SocketServer::getBikeData() const {
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < bikeData.size(); ++i) {
        if (i > 0) oss << ",";
        oss << bikeData[i];
    }
    oss << "]";
    return oss.str();
}

void SocketServer::clearBikeData() {
    bikeData.clear();
}

// SocketClient Implementation
SocketClient::SocketClient(const std::string& host, int port) 
    : clientSocket(INVALID_SOCKET), serverHost(host), serverPort(port) {
}

SocketClient::~SocketClient() {
    disconnect();
    cleanupWinsock();
}

void SocketClient::initializeWinsock() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
    }
#endif
}

void SocketClient::cleanupWinsock() {
#ifdef _WIN32
    WSACleanup();
#endif
}

bool SocketClient::initialize() {
    initializeWinsock();
    
    clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create client socket" << std::endl;
        return false;
    }
    
    std::cout << "UDP Client initialized" << std::endl;
    return true;
}

void SocketClient::disconnect() {
    if (clientSocket != INVALID_SOCKET) {
#ifdef _WIN32
        closesocket(clientSocket);
#else
        close(clientSocket);
#endif
        clientSocket = INVALID_SOCKET;
    }
}

bool SocketClient::sendJoinMessage(const std::string& bikeId) {
    Message msg = messageHandler.createJoinMessage(bikeId);
    return sendUDP(messageHandler.serializeMessage(msg));
}

bool SocketClient::sendDataMessage(const std::string& bikeId, const std::string& gpsData) {
    Message msg = messageHandler.createDataMessage(bikeId, gpsData);
    return sendUDP(messageHandler.serializeMessage(msg));
}

bool SocketClient::sendCommandMessage(const std::string& bikeId, const std::string& command) {
    Message msg = messageHandler.createCommandMessage(bikeId, command);
    return sendUDP(messageHandler.serializeMessage(msg));
}

bool SocketClient::sendSetupMessage(const std::string& bikeId, const std::string& config) {
    Message msg = messageHandler.createSetupMessage(bikeId, config);
    return sendUDP(messageHandler.serializeMessage(msg));
}

std::string SocketClient::receiveMessage() {
    char buffer[1024];
    struct sockaddr_in serverAddr;
    int serverAddrLen = sizeof(serverAddr);
    
    int bytesReceived = recvfrom(clientSocket, buffer, sizeof(buffer) - 1, 0,
                                (struct sockaddr*)&serverAddr, &serverAddrLen);
    
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        return std::string(buffer);
    }
    
    return "";
}

bool SocketClient::sendUDP(const std::string& message) {
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    serverAddr.sin_addr.s_addr = inet_addr(serverHost.c_str());
    
    int bytesSent = sendto(clientSocket, message.c_str(), message.length(), 0,
                          (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    return bytesSent > 0;
} 