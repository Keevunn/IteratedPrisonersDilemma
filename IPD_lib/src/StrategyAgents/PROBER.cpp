#include "../../include/StrategyAgents/PROBER.h"

namespace StrategyAgents {

    void PROBER::addToScore(const double value) {
        if (isProbing)
            lastPayoff = value;
        Agent::addToScore(value);
    }

    // Payoff change =  T -> R represents no retaliation
    ResponseType PROBER::decideLogic(const ResponseType& lastResponse) {
        if (currentRound > probePhase.size()) isProbing = false;
        if (!isProbing) return (canExploit ? ResponseType::D : lastResponse);

        if (lastPayoff == GameConfig::payoffs[0]) canExploit = true;
        if (canExploit && lastPayoff != GameConfig::payoffs[1]) canExploit = false;

        return probePhase[currentRound++];
    }

    std::ostream& operator<<(std::ostream& os, const PROBER& agent) {
        os << std::string("PROBER");
        return os;
    }


}