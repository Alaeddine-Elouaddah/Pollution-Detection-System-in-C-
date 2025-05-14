#include "sensor.h"
#include <iomanip>
#include <sstream>

// Constructor
PollutionSensor::PollutionSensor(const std::string& id, const std::string& loc) 
    : sensorId(id), location(loc) {}

// Determine pollution level based on AQI
PollutionLevel PollutionSensor::determinePollutionLevel(double aqi) const {
    if (aqi <= 50) return PollutionLevel::GOOD;
    else if (aqi <= 100) return PollutionLevel::MODERATE;
    else if (aqi <= 150) return PollutionLevel::UNHEALTHY;
    else if (aqi <= 200) return PollutionLevel::VERY_UNHEALTHY;
    else return PollutionLevel::HAZARDOUS;
}

// Convert enum to string
std::string PollutionSensor::levelToString(PollutionLevel level) const {
    switch(level) {
        case PollutionLevel::GOOD: return "Good";
        case PollutionLevel::MODERATE: return "Moderate";
        case PollutionLevel::UNHEALTHY: return "Unhealthy";
        case PollutionLevel::VERY_UNHEALTHY: return "Very Unhealthy";
        case PollutionLevel::HAZARDOUS: return "Hazardous";
        default: return "Unknown";
    }
}

// Add a new reading
void PollutionSensor::addReading(double aqi, double co2, double pm2_5, double pm10, double no2) {
    SensorReading reading;
    reading.timestamp = time(nullptr);
    reading.airQualityIndex = aqi;
    reading.co2Level = co2;
    reading.pm2_5 = pm2_5;
    reading.pm10 = pm10;
    reading.no2Level = no2;
    reading.level = determinePollutionLevel(aqi);
    
    readings.push_back(reading);
}

// Display the latest reading
void PollutionSensor::displayLatestReading() const {
    if (readings.empty()) {
        std::cout << "No readings available.\n";
        return;
    }
    
    const SensorReading& latest = readings.back();
    std::cout << "\n=== Latest Pollution Reading ===\n";
    std::cout << "Sensor ID: " << sensorId << "\n";
    std::cout << "Location: " << location << "\n";
    std::cout << "Timestamp: " << ctime(&latest.timestamp);
    std::cout << "AQI: " << latest.airQualityIndex << "\n";
    std::cout << "CO2: " << latest.co2Level << " ppm\n";
    std::cout << "PM2.5: " << latest.pm2_5 << " μg/m³\n";
    std::cout << "PM10: " << latest.pm10 << " μg/m³\n";
    std::cout << "NO2: " << latest.no2Level << " ppb\n";
    std::cout << "Level: " << levelToString(latest.level) << "\n";
    std::cout << "==============================\n";
}

// Display all readings
void PollutionSensor::displayAllReadings() const {
    std::cout << "\n=== All Pollution Readings ===\n";
    std::cout << "Sensor ID: " << sensorId << "\n";
    std::cout << "Location: " << location << "\n";
    std::cout << "Total Readings: " << readings.size() << "\n\n";
    
    for (const auto& reading : readings) {
        std::cout << "Timestamp: " << ctime(&reading.timestamp);
        std::cout << "AQI: " << reading.airQualityIndex << " | ";
        std::cout << "Level: " << levelToString(reading.level) << "\n";
        std::cout << "--------------------------------\n";
    }
}

// Generate a report file
void PollutionSensor::generateReport(const std::string& filename) const {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error creating report file.\n";
        return;
    }
    
    outFile << "Pollution Sensor Report\n";
    outFile << "Sensor ID: " << sensorId << "\n";
    outFile << "Location: " << location << "\n";
    outFile << "Total Readings: " << readings.size() << "\n\n";
    
    outFile << std::left << std::setw(25) << "Timestamp" 
            << std::setw(10) << "AQI" 
            << std::setw(20) << "Level"
            << std::setw(10) << "CO2"
            << std::setw(10) << "PM2.5"
            << std::setw(10) << "PM10"
            << std::setw(10) << "NO2" << "\n";
    
    for (const auto& reading : readings) {
        char timeStr[26];
        ctime_s(timeStr, sizeof(timeStr), &reading.timestamp);
        timeStr[24] = '\0'; // Remove newline
        
        outFile << std::left << std::setw(25) << timeStr
                << std::setw(10) << reading.airQualityIndex
                << std::setw(20) << levelToString(reading.level)
                << std::setw(10) << reading.co2Level
                << std::setw(10) << reading.pm2_5
                << std::setw(10) << reading.pm10
                << std::setw(10) << reading.no2Level << "\n";
    }
    
    outFile.close();
    std::cout << "Report generated: " << filename << "\n";
}

// Calculate average AQI
double PollutionSensor::getAverageAQI() const {
    if (readings.empty()) return 0.0;
    
    double sum = 0.0;
    for (const auto& reading : readings) {
        sum += reading.airQualityIndex;
    }
    return sum / readings.size();
}

// Get worst reading
PollutionLevel PollutionSensor::getWorstReading() const {
    if (readings.empty()) return PollutionLevel::GOOD;
    
    auto maxElement = std::max_element(readings.begin(), readings.end(),
        [](const SensorReading& a, const SensorReading& b) {
            return a.airQualityIndex < b.airQualityIndex;
        });
    
    return maxElement->level;
}

// Get critical readings (Unhealthy or worse)
std::vector<SensorReading> PollutionSensor::getCriticalReadings() const {
    std::vector<SensorReading> critical;
    
    for (const auto& reading : readings) {
        if (reading.level >= PollutionLevel::UNHEALTHY) {
            critical.push_back(reading);
        }
    }
    
    return critical;
}
