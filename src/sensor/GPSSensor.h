#ifndef GPS_SENSOR_H
#define GPS_SENSOR_H

#include <string>
#include <chrono>
#include <random>

class GPSSensor {
private:
    double latitude;
    double longitude;
    std::chrono::system_clock::time_point timestamp;
    std::mt19937 rng;
    std::uniform_real_distribution<double> lat_dist;
    std::uniform_real_distribution<double> lon_dist;
    
    // Default coordinates for simulation (around a city center)
    static constexpr double DEFAULT_LAT = 40.7128;  // New York City latitude
    static constexpr double DEFAULT_LON = -74.0060; // New York City longitude
    static constexpr double VARIATION = 0.01;       // ~1km variation

public:
    // Constructor
    GPSSensor();
    
    // Generate new GPS reading
    void updateReading();
    
    // Get current GPS data
    double getLatitude() const;
    double getLongitude() const;
    std::string getTimestamp() const;
    
    // Generate JSON format of current reading
    std::string toJSON() const;
    
    // Set specific coordinates (for testing)
    void setCoordinates(double lat, double lon);
};

#endif // GPS_SENSOR_H 