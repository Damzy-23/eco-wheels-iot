#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "../src/sensor/GPSSensor.h"
#include <string>
#include <regex>

TEST_CASE("GPSSensor Constructor", "[gps-sensor]") {
    GPSSensor sensor;
    
    SECTION("Default coordinates should be within expected range") {
        double lat = sensor.getLatitude();
        double lon = sensor.getLongitude();
        
        // Check that coordinates are within reasonable bounds around NYC
        REQUIRE(lat >= 40.7028);  // DEFAULT_LAT - VARIATION
        REQUIRE(lat <= 40.7228);  // DEFAULT_LAT + VARIATION
        REQUIRE(lon >= -74.0160); // DEFAULT_LON - VARIATION
        REQUIRE(lon <= -73.9960); // DEFAULT_LON + VARIATION
    }
    
    SECTION("Timestamp should be valid") {
        std::string timestamp = sensor.getTimestamp();
        REQUIRE(!timestamp.empty());
        
        // Check timestamp format: YYYY-MM-DD HH:MM:SS
        std::regex timestamp_pattern(R"(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2})");
        REQUIRE(std::regex_match(timestamp, timestamp_pattern));
    }
}

TEST_CASE("GPSSensor JSON Format", "[gps-sensor]") {
    GPSSensor sensor;
    
    SECTION("JSON output should contain all required fields") {
        std::string json = sensor.toJSON();
        
        // Check that JSON contains latitude, longitude, and timestamp
        REQUIRE(json.find("\"latitude\"") != std::string::npos);
        REQUIRE(json.find("\"longitude\"") != std::string::npos);
        REQUIRE(json.find("\"timestamp\"") != std::string::npos);
    }
    
    SECTION("JSON should be valid format") {
        std::string json = sensor.toJSON();
        
        // Check JSON structure
        REQUIRE(json.find("{") == 0);  // Starts with {
        REQUIRE(json.find("}") == json.length() - 1);  // Ends with }
        REQUIRE(json.find(",") != std::string::npos);  // Contains comma separator
        
        // Check that values are properly formatted
        REQUIRE(json.find(":") != std::string::npos);  // Contains colon separators
    }
    
    SECTION("JSON should contain numeric coordinates") {
        std::string json = sensor.toJSON();
        
        // Extract latitude and longitude values
        size_t lat_pos = json.find("\"latitude\":");
        size_t lon_pos = json.find("\"longitude\":");
        
        REQUIRE(lat_pos != std::string::npos);
        REQUIRE(lon_pos != std::string::npos);
        
        // Check that values after colon are numeric
        size_t lat_value_start = json.find(":", lat_pos) + 1;
        size_t lat_value_end = json.find(",", lat_value_start);
        std::string lat_value = json.substr(lat_value_start, lat_value_end - lat_value_start);
        
        // Remove whitespace and check if it's a valid number
        lat_value.erase(0, lat_value.find_first_not_of(" \t"));
        REQUIRE(!lat_value.empty());
        REQUIRE(std::stod(lat_value) != 0.0);  // Should be a valid number
    }
    
    SECTION("Set coordinates should update JSON output") {
        double test_lat = 51.5074;  // London latitude
        double test_lon = -0.1278;  // London longitude
        
        sensor.setCoordinates(test_lat, test_lon);
        std::string json = sensor.toJSON();
        
        // Check that new coordinates appear in JSON
        std::string expected_lat = std::to_string(test_lat);
        std::string expected_lon = std::to_string(test_lon);
        
        REQUIRE(json.find(expected_lat.substr(0, 8)) != std::string::npos);  // Check first 8 chars
        REQUIRE(json.find(expected_lon.substr(0, 8)) != std::string::npos);  // Check first 8 chars
    }
} 