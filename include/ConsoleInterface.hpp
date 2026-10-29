#ifndef CONSOLE_INTERFACE_HPP
#define CONSOLE_INTERFACE_HPP

#include <iosfwd>
#include "Common.hpp"
#include "SimulationEngine.hpp"

// Menu-driven console UI (SAD 4.5). Talks only to SimulationEngine.
// Streams are injected so the UI can be driven by scripted input in tests.
class ConsoleInterface {
public:
    ConsoleInterface(SimulationEngine& engine, std::istream& in, std::ostream& out);
    void run();   // returns on Exit (0) or when input ends

private:
    SimulationEngine& engine;
    std::istream& in;
    std::ostream& out;
    bool autoGenEnabled = false;
    int  genPercent = 30;           // chance per tick, 1-100

    void printMenu() const;
    void displayStatus() const;
    bool selectApproach(Approach& result);
    void runBatch();
    void maybeGenerateVehicle();

    void handleStart();
    void handlePause();
    void handleResume();
    void handleReset();
    void handleStop();
    void handleAddVehicle();
    void handleToggleAutoGeneration();
    void handleConfigureDurations();
    void handleSignalMode();
    void handleViewStatus();
    void handleViewStatistics();
};

#endif // CONSOLE_INTERFACE_HPP
