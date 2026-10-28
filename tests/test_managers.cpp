
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

    // TC-SIG-01: Verify the initial North-South phase.
    sm.setDurations(3, 2);
    sm.changeSignal(0);

    CHECK(sm.getSignalState(Approach::NORTH) == SignalColor::GREEN &&
          sm.getSignalState(Approach::SOUTH) == SignalColor::GREEN,
          "TC-SIG-01: North and South green at tick 0");

    CHECK(sm.getSignalState(Approach::EAST) == SignalColor::RED &&
          sm.getSignalState(Approach::WEST) == SignalColor::RED,
          "TC-SIG-01: East and West red at tick 0");

    // TC-SIG-02: North-South green lasts for the configured duration.
    sm.changeSignal(2);

    CHECK(sm.getSignalState(Approach::NORTH) == SignalColor::GREEN &&
          sm.getSignalState(Approach::SOUTH) == SignalColor::GREEN,
          "TC-SIG-02: North-South remains green before transition");

    // Verify that opposing directions never have simultaneous permission.
    auto safe = [&sm](const char* label) {
        bool northSouth =
            sm.getSignalState(Approach::NORTH) != SignalColor::RED ||
            sm.getSignalState(Approach::SOUTH) != SignalColor::RED;

        bool eastWest =
            sm.getSignalState(Approach::EAST) != SignalColor::RED ||
            sm.getSignalState(Approach::WEST) != SignalColor::RED;

        CHECK(!(northSouth && eastWest), label);
    };

    safe("TC-SAFE-01: No conflicting approaches at tick 2");

    // TC-SIG-03: Check East-West after the North-South phase ends.
    sm.changeSignal(5);

    CHECK(sm.getSignalState(Approach::EAST) == SignalColor::GREEN &&
          sm.getSignalState(Approach::WEST) == SignalColor::GREEN,
          "TC-SIG-03: East-West green after North-South phase");

    safe("TC-SAFE-01: No conflicting approaches at tick 5");

    // TC-SIG-04: Check the East-West phase and automatic cycle.
    sm.changeSignal(7);

    CHECK(sm.getSignalState(Approach::EAST) == SignalColor::GREEN &&
          sm.getSignalState(Approach::WEST) == SignalColor::GREEN,
          "TC-SIG-04: East-West remains green before transition");

    safe("TC-SAFE-01: No conflicting approaches at tick 7");

    sm.changeSignal(10);

    CHECK(sm.getSignalState(Approach::NORTH) == SignalColor::GREEN &&
          sm.getSignalState(Approach::SOUTH) == SignalColor::GREEN,
          "Automatic cycle returns to North-South");

    safe("TC-SAFE-01: No conflicting approaches at tick 10");

    // TC-SIG-03: Invalid duration values must not replace valid durations.
    sm.reset();
    sm.setDurations(3, 2);
    sm.setDurations(0, -1);
    sm.changeSignal(5);

    CHECK(sm.getSignalState(Approach::EAST) == SignalColor::GREEN &&
          sm.getSignalState(Approach::WEST) == SignalColor::GREEN,
          "TC-SIG-03: Invalid durations are ignored");

    // TC-SIG-04: Manual selection should remain safe.
    sm.toggleManualMode(true);
    sm.setManualPhase(Approach::WEST);
    sm.changeSignal(6);

    CHECK(sm.getSignalState(Approach::WEST) == SignalColor::GREEN,
          "TC-SIG-04: Manual mode selects West");

    safe("TC-SAFE-01: No conflicting approaches in manual mode");

    // Automatic operation should resume after manual mode is disabled.
    sm.toggleManualMode(false);
    sm.changeSignal(0);

    CHECK(sm.getSignalState(Approach::NORTH) == SignalColor::GREEN &&
          sm.getSignalState(Approach::SOUTH) == SignalColor::GREEN,
          "Automatic mode resumes after manual mode");

    std::cout << "\nPassed: " << passed
              << " | Failed: " << failed << '\n';

    return failed == 0 ? 0 : 1;
}
