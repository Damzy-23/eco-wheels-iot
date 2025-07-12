#include "GPSSensor.h"
#include <sstream>
#include <iomanip>

GPSSensor::GPSSensor() 
    : latitude(DEFAULT_LAT), 
      longitude(DEFAULT_LON),
      timestamp(std::chrono::system_clock::now()),
      rng(std::random_device{}()),
      lat_dist(DEFAULT_LAT - VARIATION, DEFAULT_LAT + VARIATION),
      lon_dist(DEFAULT_LON - VARIATION, DEFAULT_LON + VARIATION) {
}

void GPSSensor::updateReading() {
    latitude = lat_dist(rng);
    longitude = lon_dist(rng);
    timestamp = std::chrono::system_clock::now();
}

double GPSSensor::getLatitude() const {
    return latitude;
}

double GPSSensor::getLongitude() const {
    return longitude;
}

std::string GPSSensor::getTimestamp() const {
    auto time_t = std::chrono::system_clock::to_time_t(timestamp);
    auto tm = *std::localtime(&time_t);
    
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::string GPSSensor::toJSON() const {
    std::ostringstream json;
    json << "{";
    json << "\"latitude\": " << std::fixed << std::setprecision(6) << latitude << ",";
    json << "\"longitude\": " << std::fixed << std::setprecision(6) << longitude << ",";
    json << "\"timestamp\": \"" << getTimestamp() << "\"";
    json << "}";
    return json.str();
}

void GPSSensor::setCoordinates(double lat, double lon) {
    latitude = lat;
    longitude = lon;
    timestamp = std::chrono::system_clock::now();
} 