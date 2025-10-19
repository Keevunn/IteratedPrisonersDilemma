#include "../../include/StrategyAgents/CONTRITE.h"

#include <iostream>

namespace StrategyAgents {

    ResponseType CONTRITE::decide(const ResponseType& lastResponse) {
        if (lastResponse == ResponseType::INVALID) return initialResponse;
        return lastResponse;
    }

}