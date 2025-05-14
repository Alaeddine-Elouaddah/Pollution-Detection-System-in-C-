#ifndef POLLUTION_SENSOR_H
#define POLLUTION_SENSOR_H

#include <vector>
#include <string>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <fstream>

// Enum for pollution levels
enum class PollutionLevel {
    GOOD,
    MODERATE,
    UNHEALTHY,
    VERY_UNHEALTHY,
    HAZARDOUS
};

// Structure for sensor data
struct SensorReading {
    time_t timestamp;
    double airQualityIndex;
    double co2Level;
    double pm2_5;
    double pm10;
    double no2Level;
    PollutionLevel level;
};

class PollutionSensor {
private:
    std::string sensorId;
    std::string location;
    std::vector<SensorReading> readings;
    
    // Private helper methods
    PollutionLevel determinePollutionLevel(double aqi) const;
    std::string levelToString(PollutionLevel level) const;
    
public:
    // Constructor
    PollutionSensor(const std::string& id, const std::string& loc);
    
    // Core functionality
    void addReading(double aqi, double co2, double pm2_5, double pm10, double no2);
    void displayLatestReading() const;
    void displayAllReadings() const;
    void generateReport(const std::string& filename) const;
    
    // Analysis methods
    double getAverageAQI() const;
    PollutionLevel getWorstReading() const;
    std::vector<SensorReading> getCriticalReadings() const;
    
    // Getters
    std::string getSensorId() const { return sensorId; }
    std::string getLocation() const { return location; }
    int getReadingCount() const { return readings.size(); }
};

#endif // POLLUTION_SENSOR_H
