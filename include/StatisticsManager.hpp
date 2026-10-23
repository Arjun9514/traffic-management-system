#ifndef STATISTICS_MANAGER_HPP
#define STATISTICS_MANAGER_HPP

#include <vector>
#include "Vehicle.hpp"

class StatisticsManager {
private:
    int totalProcessed = 0;
    long long totalWaitTime = 0;
public:
    void recordProcessedVehicle(const Vehicle& v, int exitTick);
    double calculateAverageWaitTime() const;
    double calculateThroughput(int totalTicks) const;
    int getProcessedCount() const;
    void reset();
    void printSummary(int totalTicks) const;
};

#endif // STATISTICS_MANAGER_HPP
