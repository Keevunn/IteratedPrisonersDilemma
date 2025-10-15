#include "../../include/StrategyAgents/PAVLOV.h"

#include <iostream>

namespace StrategyAgents {

    void PAVLOV::addToScore(const double value) {
        lastPayoff = value;
        Agent::addToScore(value);
    }

    ResponseType PAVLOV::decideLogic(const ResponseType& lastResponse) {
        if (lastResponse == ResponseType::INVALID) return response;
        response = (lastPayoff == GameConfig::payoffs[1] || lastPayoff == GameConfig::payoffs[2]) ?
                        response : (response == ResponseType::C) ? ResponseType::D : ResponseType::C;
        return response;
    }

    std::ostream& operator<<(std::ostream& os, const PAVLOV& agent) {
        os << std::string("Pavlov: Win-Stay, Lose-Shift");
        return os;
    }

}