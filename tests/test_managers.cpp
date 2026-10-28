#include <iostream>
#include "../include/Vehicle.hpp"
#include "../include/QueueManager.hpp"
#include "../include/SignalManager.hpp"
#include "../include/Common.hpp"

static int passed = 0;
static int failed = 0;

#define CHECK(condition, message) do { \
    if (condition) { \
        ++passed; \
        std::cout << "PASS: " << message << '\n'; \
    } else { \
        ++failed; \
        std::cout << "FAIL: " << message << '\n'; \
    } \
} while (false)

int main() {
    std::cout << "=== TC-VM: Vehicle Tests ===\n";

    VehicleManager vm;
    Vehicle v1 = vm.createVehicle(Approach::NORTH, 0);
    Vehicle v2 = vm.createVehicle(Approach::SOUTH, 2);

    CHECK(v1.id != v2.id, "Vehicle IDs are unique");
    CHECK(v1.entryTick == 0, "First vehicle entry tick");
    CHECK(v2.entryTick == 2, "Second vehicle entry tick");
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
          "Total waiting count");

    Vehicle out{};
    CHECK(qm.dequeueVehicle(Approach::NORTH, out),
          "Dequeue from non-empty queue");
    CHECK(out.id == v1.id, "FIFO order");
    CHECK(!qm.dequeueVehicle(Approach::NORTH, out),
          "Empty queue returns false");

    qm.reset();
    CHECK(qm.getTotalWaitingCount() == 0,
          "Queue reset clears vehicles");

    std::cout << "\n=== TC-SIG: Signal Tests ===\n";

    SignalManager sm;
    sm.reset();
    sm.setDurations(2, 1);

    // Safety: opposing axes must never both show GREEN or YELLOW.
    auto safe = [&sm](const char* label) {
        auto active = [&sm](Approach a) {
            return sm.getSignalState(a) != SignalColor::RED;
        };
        bool northSouth = active(Approach::NORTH) || active(Approach::SOUTH);
        bool eastWest = active(Approach::EAST) || active(Approach::WEST);
        CHECK(!(northSouth && eastWest), label);
    };

    sm.changeSignal(0);
    CHECK(sm.getSignalState(Approach::NORTH) == SignalColor::GREEN &&
          sm.getSignalState(Approach::SOUTH) == SignalColor::GREEN,
          "TC-SIG-01: North and South green at tick 0");
    safe("TC-SAFE-01: No conflicting green at tick 0");

    sm.changeSignal(2);
    CHECK(sm.getSignalState(Approach::NORTH) == SignalColor::YELLOW &&
          sm.getSignalState(Approach::SOUTH) == SignalColor::YELLOW,
          "TC-SIG-02: North-South yellow after green duration");
    safe("Safety during North-South yellow");

    sm.changeSignal(3);
    CHECK(sm.getSignalState(Approach::EAST) == SignalColor::GREEN &&
          sm.getSignalState(Approach::WEST) == SignalColor::GREEN,
          "TC-SIG-03: East and West green after North-South yellow");
    safe("Safety during East-West green");

    sm.changeSignal(5);
    CHECK(sm.getSignalState(Approach::EAST) == SignalColor::YELLOW &&
          sm.getSignalState(Approach::WEST) == SignalColor::YELLOW,
          "TC-SIG-04: East-West yellow after green duration");
    safe("Safety during East-West yellow");

    sm.changeSignal(6);
    CHECK(sm.getSignalState(Approach::NORTH) == SignalColor::GREEN,
          "TC-SIG-05: Cycle returns to North-South");

    sm.toggleManualMode(true);
    sm.setManualPhase(Approach::WEST);
    sm.changeSignal(3);
    CHECK(sm.getSignalState(Approach::WEST) == SignalColor::GREEN,
          "Manual mode preserves selected phase");
    safe("Manual mode safety");

    sm.toggleManualMode(false);
    sm.changeSignal(0);
    CHECK(sm.getSignalState(Approach::NORTH) == SignalColor::GREEN,
          "Automatic mode resumes after manual mode");

    std::cout << "\nPassed: " << passed
              << " | Failed: " << failed << '\n';

    return failed == 0 ? 0 : 1;
}