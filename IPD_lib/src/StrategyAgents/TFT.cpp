#include "../../include/StrategyAgents/TFT.h"

#include <iostream>

namespace StrategyAgents {

    ResponseType TFT::decide(const ResponseType& lastResponse) {
        if (lastResponse == ResponseType::INVALID) return noisyResponse(initialResponse);
        return noisyResponse(lastResponse);
    }

}