#include "../../include/StrategyAgents/ALLD.h"

#include <iostream>

namespace StrategyAgents {

    ResponseType ALLD::decideLogic(const ResponseType& lastResponse) {
        return ResponseType::D;
    }

    std::ostream& operator<<(std::ostream& os, const ALLD& agent) {
        os << std::string("Always Defect");
        return os;
    }


}