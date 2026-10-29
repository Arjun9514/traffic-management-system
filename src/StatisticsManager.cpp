#include "StatisticsManager.hpp"
#include <iostream>
#include <iomanip>

void StatisticsManager::recordProcessedVehicle(const Vehicle& v, int exitTick) {
    long long wait = static_cast<long long>(exitTick) - v.entryTick;
    if (wait < 0) {
        std::cerr << "[Statistics] Warning: negative wait time for vehicle "
                  << "(entryTick=" << v.entryTick << ", exitTick=" << exitTick
                  << "). Clamping to 0.\n";
        wait = 0;                    // guard against bad timestamps
    }
    totalWaitTime += wait;
    totalProcessed++;
}

double StatisticsManager::calculateAverageWaitTime() const {
    if (totalProcessed == 0) return 0.0;   // avoid divide-by-zero
    return static_cast<double>(totalWaitTime) / totalProcessed;
}

double StatisticsManager::calculateThroughput(int totalTicks) const {
    if (totalTicks <= 0) return 0.0;       // avoid divide-by-zero
    return static_cast<double>(totalProcessed) / totalTicks;  // vehicles per tick
}

int StatisticsManager::getProcessedCount() const {
    return totalProcessed;
}

long long StatisticsManager::getTotalWaitTime() const {
    return totalWaitTime;
}

void StatisticsManager::reset() {
    totalProcessed = 0;
    totalWaitTime = 0;
}

void StatisticsManager::printSummary(int totalTicks) const {
    // Save formatting state so we don't affect the rest of the program
    std::ios_base::fmtflags oldFlags = std::cout.flags();
    std::streamsize oldPrecision = std::cout.precision();

    std::cout << "\n--- Simulation Statistics ---\n"
              << "Total ticks:        " << totalTicks << "\n"
              << "Vehicles processed: " << totalProcessed << "\n"
              << std::fixed << std::setprecision(2)
              << "Average wait time:  " << calculateAverageWaitTime() << " ticks\n"
              << std::setprecision(4)
              << "Throughput:         " << calculateThroughput(totalTicks) << " vehicles/tick\n";

    // Restore formatting state
    std::cout.flags(oldFlags);
    std::cout.precision(oldPrecision);
}
