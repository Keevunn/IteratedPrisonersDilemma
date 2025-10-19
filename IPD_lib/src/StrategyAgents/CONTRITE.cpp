#include "../../include/StrategyAgents/CONTRITE.h"

#include <iostream>

namespace StrategyAgents {

    ResponseType CONTRITE::decide(const ResponseType& lastResponse) {
        ResponseType original;

        if (shouldApologise) {
            shouldApologise = false;
            original = ResponseType::C;
        }
        else
            original = (lastResponse == ResponseType::INVALID) ? initialResponse : lastResponse;

        ResponseType newResponse = noisyResponse(original);

        if (original != newResponse) shouldApologise = true;
        return newResponse;
    }

    void CONTRITE::resetAgent() {
        Agent::resetAgent();
        shouldApologise = false;
    }
}
