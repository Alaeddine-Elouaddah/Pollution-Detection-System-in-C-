#include "sensor.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

// Function prototypes
void displayMenu();
void simulateSensorReadings(PollutionSensor& sensor, int count);

int main() {
    // Initialize random seed
    std::srand(std::time(nullptr));
    
    // Create a pollution sensor
    PollutionSensor citySensor("PS-1000", "Downtown City Center");
    
    int choice;
    do {
        displayMenu();
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        
        switch(choice) {
            case 1: {
                // Simulate readings
                int count;
                std::cout << "How many readings to simulate? ";
                std::cin >> count;
                simulateSensorReadings(citySensor, count);
                break;
            }
            case 2:
                citySensor.displayLatestReading();
                break;
            case 3:
                citySensor.displayAllReadings();
                break;
            case 4: {
                std::string filename;
                std::cout << "Enter report filename: ";
                std::cin >> filename;
                citySensor.generateReport(filename);
                break;
            }
            case 5:
                std::cout << "Average AQI: " << citySensor.getAverageAQI() << "\n";
                break;
            case 6: {
                auto worst = citySensor.getWorstReading();
                std::cout << "Worst pollution level recorded: " 
                          << citySensor.levelToString(worst) << "\n";
                break;
            }
            case 7: {
                auto critical = citySensor.getCriticalReadings();
                std::cout << "Critical readings (Unhealthy or worse): " 
                          << critical.size() << "\n";
                break;
            }
            case 8:
                std::cout << "Exiting program...\n";
                break;
            default:
                std::cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 8);
    
    return 0;
}

// Display menu options
void displayMenu() {
    std::cout << "\n=== Pollution Monitoring System ===\n";
    std::cout << "1. Simulate sensor readings\n";
    std::cout << "2. Display latest reading\n";
    std::cout << "3. Display all readings\n";
    std::cout << "4. Generate report\n";
    std::cout << "5. Show average AQI\n";
    std::cout << "6. Show worst reading\n";
    std::cout << "7. Show critical readings count\n";
    std::cout << "8. Exit\n";
}

// Simulate random sensor readings
void simulateSensorReadings(PollutionSensor& sensor, int count) {
    for (int i = 0; i < count; ++i) {
        // Generate random values within realistic ranges
        double aqi = 20 + (std::rand() % 300); // 20-320
        double co2 = 300 + (std::rand() % 1000); // 300-1300 ppm
        double pm2_5 = 5 + (std::rand() % 150); // 5-155 μg/m³
        double pm10 = 10 + (std::rand() % 200); // 10-210 μg/m³
        double no2 = 10 + (std::rand() % 100); // 10-110 ppb
        
        // Add slight time variation between readings
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        sensor.addReading(aqi, co2, pm2_5, pm10, no2);
    }
    
    std::cout << "Added " << count << " simulated readings.\n";
}
