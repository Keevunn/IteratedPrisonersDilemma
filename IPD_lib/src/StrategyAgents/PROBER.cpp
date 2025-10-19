#include "../../include/StrategyAgents/PROBER.h"

namespace StrategyAgents {

    void PROBER::addToScore(const double value) {
        if (isProbing)
            lastPayoff = value;
        Agent::addToScore(value);
    }

    // Payoff change =  T -> R represents no retaliation
    ResponseType PROBER::decide(const ResponseType& lastResponse) {
        if (currentRound >= probePhase.size()) isProbing = false;
        if (!isProbing) return noisyResponse(canExploit ? ResponseType::D : lastResponse);

        if (lastPayoff == GameConfig::GameConfig::payoffs[0]) canExploit = true;
        if (canExploit && lastPayoff != GameConfig::GameConfig::payoffs[1]) canExploit = false;

        return noisyResponse(probePhase[currentRound++]);
    }

    void PROBER::resetAgent() {
        Agent::resetAgent();
        currentRound = 0;
        lastPayoff = 0;
        isProbing = true;
        canExploit = false;
    }
}
