#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <iostream>
#include <sstream>

// Forward declarations for the classes we're testing
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
    std::string bikeId;
    
public:
    EBikeClient(const std::string& id) : bikeId(id) {}
    
    bool initializeHAL(int port) {
        return hal.attach(port);
    }
    
    void releaseHAL() {
        hal.release();
    }
    
    bool isHALAttached() const {
        return hal.isPortAttached();
    }
    
    int getHALPort() const {
        return hal.getPort();
    }
};

TEST_CASE("EBikeClient HAL Port Management", "[ebike-client]") {
    EBikeClient client("test-bike");
    
    SECTION("HAL should attach to specified port") {
        REQUIRE(client.initializeHAL(8080));
        REQUIRE(client.isHALAttached());
        REQUIRE(client.getHALPort() == 8080);
    }
    
    SECTION("HAL should release properly") {
        REQUIRE(client.initializeHAL(8080));
        REQUIRE(client.isHALAttached());
        
        client.releaseHAL();
        REQUIRE_FALSE(client.isHALAttached());
        REQUIRE(client.getHALPort() == -1);
    }
    
    SECTION("HAL should not attach twice") {
        REQUIRE(client.initializeHAL(8080));
        REQUIRE_FALSE(client.initializeHAL(8081));  // Should fail
        REQUIRE(client.getHALPort() == 8080);  // Should still be on original port
    }
    
    SECTION("HAL should not release when not attached") {
        REQUIRE_FALSE(client.isHALAttached());
        client.releaseHAL();  // Should not crash
        REQUIRE_FALSE(client.isHALAttached());
    }
    
    SECTION("HAL should work with different port numbers") {
        REQUIRE(client.initializeHAL(1234));
        REQUIRE(client.getHALPort() == 1234);
        
        client.releaseHAL();
        REQUIRE(client.initializeHAL(5678));
        REQUIRE(client.getHALPort() == 5678);
    }
} 