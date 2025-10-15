#include "../../include/StrategyAgents/TFT.h"

#include <iostream>

namespace StrategyAgents {

    ResponseType TFT::decideLogic(const ResponseType& lastResponse) {
        if (lastResponse == ResponseType::INVALID) return initialResponse;
        return lastResponse;
    }

    std::ostream& operator<<(std::ostream& os, const TFT& agent) {
        os << std::string("Tit-for-Tat");
        return os;
    }


}