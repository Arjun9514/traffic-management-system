#ifndef STATISTICS_MANAGER_HPP
#define STATISTICS_MANAGER_HPP

#include "Vehicle.hpp"

class StatisticsManager {
private:
    int totalProcessed = 0;
    long long totalWaitTime = 0;

public:
    void recordProcessedVehicle(const Vehicle& v, int exitTick);

    [[nodiscard]] double calculateAverageWaitTime() const;
    [[nodiscard]] double calculateThroughput(int totalTicks) const;
    [[nodiscard]] int getProcessedCount() const;
    [[nodiscard]] long long getTotalWaitTime() const;

    void reset();
    void printSummary(int totalTicks) const;
};

#endif // STATISTICS_MANAGER_HPP