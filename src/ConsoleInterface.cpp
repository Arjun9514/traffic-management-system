#include "ConsoleInterface.hpp"
#include "InputValidator.hpp"
#include <iomanip>
#include <iostream>

namespace {

// Input limits. Adjust to match the SRS values if they differ.
constexpr int MIN_GREEN = 1;
constexpr int MAX_GREEN = 120;
constexpr int MIN_YELLOW = 1;
constexpr int MAX_YELLOW = 10;
constexpr int MAX_RUN_TICKS = 1000;

const Approach ALL_APPROACHES[] = {
    Approach::NORTH, Approach::SOUTH, Approach::EAST, Approach::WEST
};

const char* approachName(Approach a) {
    switch (a) {
        case Approach::NORTH: return "North";
        case Approach::SOUTH: return "South";
        case Approach::EAST:  return "East";
        case Approach::WEST:  return "West";
    }
    return "Unknown";
}

const char* colorName(SignalColor c) {
    switch (c) {
        case SignalColor::RED:    return "RED";
        case SignalColor::YELLOW: return "YELLOW";
        case SignalColor::GREEN:  return "GREEN";
    }
    return "UNKNOWN";
}

const char* stateName(SimState s) {
    switch (s) {
        case SimState::STOPPED: return "STOPPED";
        case SimState::RUNNING: return "RUNNING";
        case SimState::PAUSED:  return "PAUSED";
    }
    return "UNKNOWN";
}

} // namespace

ConsoleInterface::ConsoleInterface(SimulationEngine& e, std::istream& i, std::ostream& o)
    : engine(e), in(i), out(o) {}

void ConsoleInterface::run() {
    while (true) {
        printMenu();
        int choice = 0;
        if (!InputValidator::readInt(in, out, "Enter choice (0-11): ", 0, 11, choice)) {
            out << "\nInput closed. Exiting.\n";
            return;
        }
        switch (choice) {
            case 0:  out << "Exiting. Goodbye.\n"; return;
            case 1:  handleStart();                break;
            case 2:  handlePause();                break;
            case 3:  handleResume();               break;
            case 4:  handleReset();                break;
            case 5:  handleStop();                 break;
            case 6:  handleAddVehicle();           break;
            case 7:  handleToggleAutoGeneration(); break;
            case 8:  handleConfigureDurations();   break;
            case 9:  handleSignalMode();           break;
            case 10: handleViewStatus();           break;
            case 11: handleViewStatistics();       break;
            default: break;   // unreachable: readInt enforces 0-11
        }
    }
}

void ConsoleInterface::printMenu() const {
    out << "\n========== Traffic Management System ==========\n"
        << " 1. Start Simulation\n"
        << " 2. Pause Simulation\n"
        << " 3. Resume Simulation\n"
        << " 4. Reset Simulation\n"
        << " 5. Stop Simulation\n"
        << " 6. Add Vehicle\n"
        << " 7. Enable/Disable Automatic Vehicle Generation\n"
        << " 8. Configure Signal Durations\n"
        << " 9. Select Automatic/Manual Signal Mode\n"
        << "10. View Current Queues and Signals\n"
        << "11. View Statistics\n"
        << " 0. Exit\n"
        << "State: " << stateName(engine.getState())
        << " | Tick: " << engine.getCurrentTick()
        << " | Auto-generation: " << (autoGenEnabled ? "ON" : "OFF") << "\n"
        << "===============================================\n";
}

void ConsoleInterface::displayStatus() const {
    std::ios_base::fmtflags oldFlags = out.flags();
    out << "\n--- Intersection Status (Tick " << engine.getCurrentTick()
        << ", " << stateName(engine.getState()) << ") ---\n"
        << std::left << std::setw(10) << "Approach"
        << std::setw(10) << "Signal" << "Queue\n";
    for (Approach a : ALL_APPROACHES) {
        out << std::setw(10) << approachName(a)
            << std::setw(10) << colorName(engine.getSignalManager().getSignalState(a))
            << engine.getQueueManager().getQueueSize(a) << "\n";
    }
    out << "Waiting vehicles:   " << engine.getQueueManager().getTotalWaitingCount() << "\n"
        << "Processed vehicles: " << engine.getStatsManager().getProcessedCount() << "\n";
    out.flags(oldFlags);
}

bool ConsoleInterface::selectApproach(Approach& result) {
    out << "  1) North   2) South   3) East   4) West\n";
    int c = 0;
    if (!InputValidator::readInt(in, out, "Select approach (1-4): ", 1, 4, c)) return false;
    result = ALL_APPROACHES[c - 1];
    return true;
}

void ConsoleInterface::maybeGenerateVehicle() {
    if (!autoGenEnabled) return;
    Approach a = Approach::NORTH;
    if (engine.getVehicleManager().generateVehicle(a, genPercent / 100.0)) {
        Vehicle v = engine.getVehicleManager().createVehicle(a, engine.getCurrentTick());
        engine.getQueueManager().enqueueVehicle(v);
    }
}

void ConsoleInterface::runBatch() {
    int n = 0;
    if (!InputValidator::readInt(in, out, "Ticks to run (1-1000): ", 1, MAX_RUN_TICKS, n)) return;
    for (int i = 0; i < n; ++i) {
        maybeGenerateVehicle();
        engine.tick();
    }
    displayStatus();
}

void ConsoleInterface::handleStart() {
    if (engine.getState() != SimState::STOPPED) {
        out << "Simulation is already running or paused. Use Reset or Stop first.\n";
        return;
    }
    engine.start();
    out << "Simulation started.\n";
    runBatch();
}

void ConsoleInterface::handlePause() {
    if (engine.getState() != SimState::RUNNING) {
        out << "Cannot pause: simulation is not running.\n";
        return;
    }
    engine.pause();
    out << "Simulation paused at tick " << engine.getCurrentTick() << ".\n";
}

void ConsoleInterface::handleResume() {
    if (engine.getState() != SimState::PAUSED) {
        out << "Cannot resume: simulation is not paused.\n";
        return;
    }
    engine.resume();
    out << "Simulation resumed.\n";
    runBatch();
}

void ConsoleInterface::handleReset() {
    engine.reset();
    out << "Simulation reset.\n";
}

void ConsoleInterface::handleStop() {
    if (engine.getState() == SimState::STOPPED) {
        out << "Simulation is already stopped.\n";
        return;
    }
    engine.stop();
    out << "Simulation stopped.\n";
    engine.getStatsManager().printSummary(engine.getCurrentTick());
}

void ConsoleInterface::handleAddVehicle() {
    Approach a = Approach::NORTH;
    if (!selectApproach(a)) return;
    Vehicle v = engine.getVehicleManager().createVehicle(a, engine.getCurrentTick());
    engine.getQueueManager().enqueueVehicle(v);
    out << "Vehicle #" << v.id << " added to the " << approachName(a)
        << " queue (queue length: " << engine.getQueueManager().getQueueSize(a) << ").\n";
}

void ConsoleInterface::handleToggleAutoGeneration() {
    if (autoGenEnabled) {
        autoGenEnabled = false;
        out << "Automatic vehicle generation disabled.\n";
        return;
    }
    int pct = 0;
    if (!InputValidator::readInt(in, out, "Generation chance per tick in percent (1-100): ",
                                 1, 100, pct)) return;
    genPercent = pct;
    autoGenEnabled = true;
    out << "Automatic vehicle generation enabled (" << genPercent << "% per tick).\n";
}

void ConsoleInterface::handleConfigureDurations() {
    int g = 0, y = 0;
    if (!InputValidator::readInt(in, out, "Green duration in ticks (1-120): ",
                                 MIN_GREEN, MAX_GREEN, g)) return;
    if (!InputValidator::readInt(in, out, "Yellow duration in ticks (1-10): ",
                                 MIN_YELLOW, MAX_YELLOW, y)) return;
    engine.getSignalManager().setDurations(g, y);
    out << "Signal durations set: green=" << g << ", yellow=" << y << ".\n";
}

void ConsoleInterface::handleSignalMode() {
    out << "  1) Automatic   2) Manual\n";
    int mode = 0;
    if (!InputValidator::readInt(in, out, "Select mode (1-2): ", 1, 2, mode)) return;
    if (mode == 1) {
        engine.getSignalManager().toggleManualMode(false);
        out << "Signal mode: automatic.\n";
        return;
    }
    Approach a = Approach::NORTH;
    out << "Which approach gets the green signal?\n";
    if (!selectApproach(a)) return;
    engine.getSignalManager().toggleManualMode(true);
    engine.getSignalManager().setManualPhase(a);
    out << "Signal mode: manual. " << approachName(a) << " is GREEN.\n";
}

void ConsoleInterface::handleViewStatus() {
    displayStatus();
}

void ConsoleInterface::handleViewStatistics() {
    engine.getStatsManager().printSummary(engine.getCurrentTick());
}
