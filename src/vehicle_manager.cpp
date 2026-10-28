#include "Vehicle.hpp"
#include <random>

Vehicle VehicleManager::createVehicle(Approach approach, int currentTick) {
    Vehicle vehicle;

    vehicle.id = nextVehicleId++;
    vehicle.approach = approach;
    vehicle.entryTick = currentTick;

    return vehicle;
}

bool VehicleManager::generateVehicle(Approach& outApproach, double probability) {
    if (probability < 0.0 || probability > 1.0) {
        return false;
    }

    static std::random_device rd;
    static std::mt19937 generator(rd());
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    if (distribution(generator) >= probability) {
        return false;
    }

    std::uniform_int_distribution<int> approachDistribution(0, 3);

    outApproach = static_cast<Approach>(
        approachDistribution(generator)
    );

    return true;
}
