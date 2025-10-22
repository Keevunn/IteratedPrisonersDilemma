#include "../../include/StrategyAgents/GRIM.h"

#include <iostream>

namespace StrategyAgents {

    ResponseType GRIM::decide(const ResponseType& lastResponse) {
        if (lastResponse == ResponseType::INVALID) return noisyResponse(initialResponse);
        if (lastResponse == ResponseType::D)
            playerHasDefected = true;
        return noisyResponse(playerHasDefected ? ResponseType::D : ResponseType::C);
    }

    void GRIM::resetAgent() {
        Agent::resetAgent();
        playerHasDefected = false;
    }
}
