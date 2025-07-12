#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

#include <string>
#include <thread>
#include <atomic>
#include <vector>
#include <map>
#include <functional>
#include "MessageHandler.h"

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    typedef SOCKET SocketType;
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    typedef int SocketType;
#endif

class SocketServer {
private:
    SocketType serverSocket;
    std::string host;
    int port;
    std::atomic<bool> running;
    std::thread listenerThread;
    MessageHandler messageHandler;
    
    // Client tracking
    std::map<std::string, struct sockaddr_in> clients;
    std::vector<std::string> bikeData;
    
    // Callback for data updates
    std::function<void(const std::string&, const std::string&)> dataCallback;
    
public:
    SocketServer(const std::string& host = "0.0.0.0", int port = 8080);
    ~SocketServer();
    
    // Server control
    bool initialize();
    void start();
    void stop();
    bool isRunning() const;
    
    // Client management
    void addClient(const std::string& bikeId, const struct sockaddr_in& clientAddr);
    void removeClient(const std::string& bikeId);
    std::vector<std::string> getConnectedClients() const;
    
    // Data handling
    void setDataCallback(std::function<void(const std::string&, const std::string&)> callback);
    void broadcastMessage(const std::string& message);
    void sendToClient(const std::string& bikeId, const std::string& message);
    
    // Utility functions
    std::string getBikeData() const;
    void clearBikeData();
    
private:
    void listenerLoop();
    void handleClientMessage(const std::string& data, const struct sockaddr_in& clientAddr);
    bool sendUDP(const std::string& message, const struct sockaddr_in& clientAddr);
    void initializeWinsock();
    void cleanupWinsock();
};

class SocketClient {
private:
    SocketType clientSocket;
    std::string serverHost;
    int serverPort;
    MessageHandler messageHandler;
    
public:
    SocketClient(const std::string& host = "127.0.0.1", int port = 8080);
    ~SocketClient();
    
    // Client control
    bool initialize();
    void disconnect();
    
    // Message sending
    bool sendJoinMessage(const std::string& bikeId);
    bool sendDataMessage(const std::string& bikeId, const std::string& gpsData);
    bool sendCommandMessage(const std::string& bikeId, const std::string& command);
    bool sendSetupMessage(const std::string& bikeId, const std::string& config);
    
    // Message receiving
    std::string receiveMessage();
    
private:
    bool sendUDP(const std::string& message);
    void initializeWinsock();
    void cleanupWinsock();
};

#endif // SOCKET_SERVER_H 