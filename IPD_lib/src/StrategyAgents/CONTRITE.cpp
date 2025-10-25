#include "../../include/StrategyAgents/CONTRITE.h"

#include <iostream>

namespace StrategyAgents {

    ResponseType CONTRITE::decide(const ResponseType& lastResponse) {
        ResponseType intended;

        if (shouldApologise && lastResponse == ResponseType::C) { // Previous round resulted in DC
            shouldApologise = false;
            intended = ResponseType::C;
            apologisingNextRound = true;
        }
        else if (apologisingNextRound && lastResponse == ResponseType::D) { // Previous round resulted in CD
            apologisingNextRound = false;
            intended = ResponseType::C;
        }
        else
            intended = (lastResponse == ResponseType::INVALID) ? initialResponse : lastResponse;

        const ResponseType actual = noisyResponse(intended);

        if (intended == ResponseType::C && actual == ResponseType::D) {
            shouldApologise = true;
            apologisingNextRound = false;
        }
        return actual;
    }

    void CONTRITE::resetAgent() {
        Agent::resetAgent();
        shouldApologise = false;
        apologisingNextRound = false;
    }
}
