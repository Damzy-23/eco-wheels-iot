#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <string>
#include <vector>

// Example utility functions for testing
namespace MiscUtils {
    std::string generateJSON(const std::string& key, const std::string& value) {
        return "{\"" + key + "\":\"" + value + "\"}";
    }
    
    std::vector<std::string> splitString(const std::string& str, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(str);
        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }
    
    bool isValidPort(int port) {
        return port > 0 && port <= 65535;
    }
}

TEST_CASE("MiscUtils JSON Generation", "[misc-utils]") {
    SECTION("generateJSON should create valid JSON") {
        std::string json = MiscUtils::generateJSON("test", "value");
        REQUIRE(json == "{\"test\":\"value\"}");
    }
    
    SECTION("generateJSON should handle empty values") {
        std::string json = MiscUtils::generateJSON("key", "");
        REQUIRE(json == "{\"key\":\"\"}");
    }
}

TEST_CASE("MiscUtils String Splitting", "[misc-utils]") {
    SECTION("splitString should split on delimiter") {
        std::vector<std::string> result = MiscUtils::splitString("a,b,c", ',');
        REQUIRE(result.size() == 3);
        REQUIRE(result[0] == "a");
        REQUIRE(result[1] == "b");
        REQUIRE(result[2] == "c");
    }
    
    SECTION("splitString should handle empty string") {
        std::vector<std::string> result = MiscUtils::splitString("", ',');
        REQUIRE(result.size() == 1);
        REQUIRE(result[0] == "");
    }
}

TEST_CASE("MiscUtils Port Validation", "[misc-utils]") {
    SECTION("isValidPort should accept valid ports") {
        REQUIRE(MiscUtils::isValidPort(1));
        REQUIRE(MiscUtils::isValidPort(8080));
        REQUIRE(MiscUtils::isValidPort(65535));
    }
    
    SECTION("isValidPort should reject invalid ports") {
        REQUIRE_FALSE(MiscUtils::isValidPort(0));
        REQUIRE_FALSE(MiscUtils::isValidPort(-1));
        REQUIRE_FALSE(MiscUtils::isValidPort(65536));
    }
} 