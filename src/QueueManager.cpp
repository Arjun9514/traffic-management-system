#include "QueueManager.hpp"

QueueManager::QueueManager() = default;

void QueueManager::enqueueVehicle(const Vehicle& v) {
    queues[v.approach].push(v);
}

bool QueueManager::dequeueVehicle(Approach app, Vehicle& outVehicle) {
    auto it = queues.find(app);

    // Never access the front of an empty queue.
    if (it == queues.end() || it->second.empty()) {
        return false;
    }

    outVehicle = it->second.front();
    it->second.pop();
    return true;
}

size_t QueueManager::getQueueSize(Approach app) const {
    auto it = queues.find(app);
    if (it == queues.end()) {
        return 0;
    }

    return it->second.size();
}

size_t QueueManager::getTotalWaitingCount() const {
    size_t total = 0;

    for (const auto& entry : queues) {
        total += entry.second.size();
    }

    return total;
}

void QueueManager::reset() {
    queues.clear();
}
