
#include "SignalManager.hpp"

SignalColor SignalManager::getSignalState(Approach app) const {
    auto it = signals.find(app);
    return it == signals.end() ? SignalColor::RED : it->second;
}

void SignalManager::changeSignal(int tick) {
    if (isManualMode || tick < 0) return;

    const int green = greenDuration;
    const int yellow = yellowDuration;
    const int cycle = 2 * (green + yellow);
    const int phase = tick % cycle;

    if (phase < green) {
        signals[Approach::NORTH] = SignalColor::GREEN;
        signals[Approach::SOUTH] = SignalColor::RED;
        signals[Approach::EAST] = SignalColor::RED;
        signals[Approach::WEST] = SignalColor::RED;
    } else if (phase < green + yellow) {
        signals[Approach::NORTH] = SignalColor::YELLOW;
        signals[Approach::SOUTH] = SignalColor::RED;
        signals[Approach::EAST] = SignalColor::RED;
        signals[Approach::WEST] = SignalColor::RED;
    } else if (phase < 2 * green + yellow) {
        signals[Approach::NORTH] = SignalColor::RED;
        signals[Approach::SOUTH] = SignalColor::RED;
        signals[Approach::EAST] = SignalColor::GREEN;
        signals[Approach::WEST] = SignalColor::RED;
    } else {
        signals[Approach::NORTH] = SignalColor::RED;
        signals[Approach::SOUTH] = SignalColor::RED;
        signals[Approach::EAST] = SignalColor::YELLOW;
        signals[Approach::WEST] = SignalColor::RED;
    }
}

void SignalManager::setManualPhase(Approach activeGreen) {
    for (auto app : {Approach::NORTH, Approach::SOUTH,
                     Approach::EAST, Approach::WEST}) {
        signals[app] =
            (app == activeGreen) ? SignalColor::GREEN : SignalColor::RED;
    }
}

void SignalManager::setDurations(int green, int yellow) {
    if (green > 0) greenDuration = green;
    if (yellow > 0) yellowDuration = yellow;
}

void SignalManager::toggleManualMode(bool enable) {
    isManualMode = enable;
}

void SignalManager::reset() {
    isManualMode = false;
    signals[Approach::NORTH] = SignalColor::GREEN;
    signals[Approach::SOUTH] = SignalColor::RED;
    signals[Approach::EAST] = SignalColor::RED;
    signals[Approach::WEST] = SignalColor::RED;
}
