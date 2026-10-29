#ifndef SIMULATION_ENGINE_HPP
#define SIMULATION_ENGINE_HPP

#include <vector>
#include "QueueManager.hpp"
#include "SignalManager.hpp"
#include "StatisticsManager.hpp"

enum class SimState { STOPPED, RUNNING, PAUSED };

class SimulationEngine {
private:
    int currentTick = 0;
    SimState state = SimState::STOPPED;
    QueueManager queueMgr;
    SignalManager signalMgr;
    StatisticsManager statsMgr;
    VehicleManager vehicleMgr;
    std::vector<Vehicle> lastProcessed;   // vehicles that crossed in the most recent tick
public:
    void runSimulation(int ticks);
    void tick();
    void start();
    void pause();
    void resume();
    void reset();
    void stop();
    int getCurrentTick() const { return currentTick; }   // number of completed ticks
    SimState getState() const { return state; }
    const std::vector<Vehicle>& getLastProcessed() const { return lastProcessed; }
    QueueManager& getQueueManager() { return queueMgr; }
    SignalManager& getSignalManager() { return signalMgr; }
    StatisticsManager& getStatsManager() { return statsMgr; }
    VehicleManager& getVehicleManager() { return vehicleMgr; }
};

#endif // SIMULATION_ENGINE_HPP