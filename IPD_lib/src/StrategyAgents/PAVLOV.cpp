#include "../../include/StrategyAgents/PAVLOV.h"

namespace StrategyAgents {

    void PAVLOV::addToScore(const double value) {
        lastPayoff = value;
        Agent::addToScore(value);
    }

    ResponseType PAVLOV::decide(const ResponseType& lastResponse) {
        if (lastResponse == ResponseType::INVALID) return noisyResponse(response);
        response = (lastPayoff == GameConfig::GameConfig::payoffs[1] || lastPayoff == GameConfig::GameConfig::payoffs[2]) ?
                        response : (response == ResponseType::C) ? ResponseType::D : ResponseType::C;
        return noisyResponse(response);
    }

    void PAVLOV::resetAgent() {
        Agent::resetAgent();
        lastPayoff = 0;
        response = initialResponse;
    }
}
