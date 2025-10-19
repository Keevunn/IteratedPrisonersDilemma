#include "../../include/StrategyAgents/PAVLOV.h"

#include <iostream>

namespace StrategyAgents {

    void PAVLOV::addToScore(const double value) {
        lastPayoff = value;
        Agent::addToScore(value);
    }

    ResponseType PAVLOV::decide(const ResponseType& lastResponse) {
        if (lastResponse == ResponseType::INVALID) return response;
        response = (lastPayoff == GameConfig::GameConfig::payoffs[1] || lastPayoff == GameConfig::GameConfig::payoffs[2]) ?
                        response : (response == ResponseType::C) ? ResponseType::D : ResponseType::C;
        return response;
    }

    void PAVLOV::resetAgent() {
        Agent::resetAgent();
        lastPayoff = 0;
        response = initialResponse;
    }
}
