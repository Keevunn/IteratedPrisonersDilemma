#include "../../include/StrategyAgents/ALLC.h"

#include <iostream>

namespace StrategyAgents {

    ResponseType ALLC::decideLogic(const ResponseType& lastResponse) {
        return ResponseType::C;
    }

    std::ostream& operator<<(std::ostream& os, const ALLC& agent) {
        os << std::string("Always Cooperate");
        return os;
    }


}