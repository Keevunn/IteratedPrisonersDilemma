#include "../../include/StrategyAgents/GRIM.h"

#include <iostream>

namespace StrategyAgents {

    ResponseType GRIM::decideLogic(const ResponseTypew& lastResponse) {
        if (lastResponse == ResponseType::INVALID) return initialResponse;
        if (lastResponse == ResponseType::D) playerHasDefected = true;
        return (playerHasDefected ? ResponseType::D : ResponseType::C);
    }

    std::ostream& operator<<(std::ostream& os, const GRIM& agent) {
        os << std::string("Grim Trigger");
        return os;
    }

}