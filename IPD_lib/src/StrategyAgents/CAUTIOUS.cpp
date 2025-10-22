#include "../../include/StrategyAgents/CAUTIOUS.h"

namespace StrategyAgents {

    void CAUTIOUS::addToScore(const double value) {
        lastPayoff = value;
        Agent::addToScore(value);
    }

    ResponseType CAUTIOUS::decide(const ResponseType& lastResponse) {
        if (freezeResponseCount > 0) {
            freezeResponseCount--;
            response = (isInitialRound) ? initialResponse : ResponseType::C;
            if (freezeResponseCount == 0 && isInitialRound) isInitialRound = false;
            return noisyResponse(response);
        }
        // Track punishment cycles (DD)
        if (lastPayoff == GameConfig::GameConfig::payoffs[2]) consecutivePunishments++;
        else consecutivePunishments = 0;

        if (consecutivePunishments >= 3) { // Attempt reconciliation after 3+ consecutive DD
            response = ResponseType::C;
            consecutivePunishments = 0;
            freezeResponseCount = MAX_FREEZE - 1;
        } else { // Otherwise use PAVLOV strategy
            response = (lastPayoff == GameConfig::GameConfig::payoffs[1] || lastPayoff == GameConfig::GameConfig::payoffs[2])
                        ? response
                        : (response == ResponseType::C)
                            ? ResponseType::D : ResponseType::C;
        }

        return noisyResponse(response);
    }

    void CAUTIOUS::resetAgent() {
        Agent::resetAgent();
        lastPayoff = 0;
        response = initialResponse;
        consecutivePunishments = 0;
        freezeResponseCount = 2;
        isInitialRound = true;
    }
}