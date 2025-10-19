#pragma once

#include "Agent.h"

namespace StrategyAgents {
    // Initial response: C
    class CONTRITE : public Agent {
    public:
        CONTRITE() : Agent("CONTRITE") {}

        ResponseType decide(const ResponseType& lastResponse) override;

    };
}