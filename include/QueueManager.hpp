#ifndef QUEUE_MANAGER_HPP
#define QUEUE_MANAGER_HPP

#include <queue>
#include <map>
#include "Vehicle.hpp"
#include "Common.hpp"

class QueueManager {
private:
    std::map<Approach, std::queue<Vehicle>> queues;
public:
    QueueManager() {};
    void enqueueVehicle(const Vehicle& v);
    bool dequeueVehicle(Approach app, Vehicle& outVehicle);
    size_t getQueueSize(Approach app) const;
    size_t getTotalWaitingCount() const;
    void reset();
};

#endif // QUEUE_MANAGER_HPP
