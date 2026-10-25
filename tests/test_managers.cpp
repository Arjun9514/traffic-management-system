
#include <cassert>
#include <iostream>

#include "../include/Vehicle.hpp"
#include "../include/QueueManager.hpp"
#include "../include/SignalManager.hpp"
#include "../include/Common.hpp"

static int passed = 0;

#define CHECK(condition, message) \
    do { \
        if (!(condition)) { \
            std::cerr << "FAIL: " << message << '\n'; \
            return 1; \
        } \
        ++passed; \
        std::cout << "PASS: " << message << '\n'; \
    } while (false)

int main() {
    std::cout << "=== TC-VM: Vehicle Tests ===\n";

    VehicleManager vm;
    Vehicle v1 = vm.createVehicle(Approach::NORTH, 0);
    Vehicle v2 = vm.createVehicle(Approach::SOUTH, 2);

    CHECK(v1.id != v2.id, "Vehicle IDs are unique");
    CHECK(v1.entryTick == 0, "Vehicle entry tick is stored");
    CHECK(v2.entryTick == 2, "Second vehicle entry tick is stored");
    CHECK(v1.approach == Approach::NORTH,
          "Vehicle approach is stored");

    std::cout << "\n=== TC-Q: Queue Tests ===\n";

    QueueManager qm;
    qm.enqueueVehicle(v1);
    qm.enqueueVehicle(v2);

    CHECK(qm.getQueueSize(Approach::NORTH) == 1,
          "North queue size");
    CHECK(qm.getQueueSize(Approach::SOUTH) == 1,
          "South queue size");
    CHECK(qm.getTotalWaitingCount() == 2,
          "Total waiting vehicle count");

    Vehicle out{};
    CHECK(qm.dequeueVehicle(Approach::NORTH, out),
          "Dequeue from non-empty queue");
    CHECK(out.id == v1.id, "FIFO order");

    CHECK(!qm.dequeueVehicle(Approach::NORTH, out),
          "Empty queue returns false");

    qm.reset();
    CHECK(qm.getTotalWaitingCount() == 0,
          "Queue reset clears waiting vehicles");

    std::cout << "\n=== TC-SIG: Signal Tests ===\n";

    SignalManager sm;
    sm.reset();

    int greenCount = 0;
    for (Approach approach : {
             Approach::NORTH, Approach::SOUTH,
             Approach::EAST, Approach::WEST}) {
        if (sm.getSignalState(approach) == SignalColor::GREEN) {
            ++greenCount;
        }
    }

    CHECK(greenCount <= 1,
          "Reset has no conflicting green signals");

    std::cout << "\nPassed checks: " << passed << '\n';
    std::cout << "Test harness completed.\n";
    return 0;
}
