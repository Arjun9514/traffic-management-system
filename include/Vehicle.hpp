#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include "Common.hpp"

struct Vehicle {
    int id;
    Approach approach;
    int entryTick;
};

class VehicleManager {
private:
    int nextVehicleId = 1;
public:
    Vehicle createVehicle(Approach approach, int currentTick);
    bool generateVehicle(Approach& outApproach, double probability);
};

#endif // VEHICLE_HPP
