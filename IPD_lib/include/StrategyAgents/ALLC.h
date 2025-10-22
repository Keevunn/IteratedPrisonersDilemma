#pragma once

#include "Agent.h"

namespace StrategyAgents {
    // Initial response: C
    class ALLC : public Agent {
    public:
        ALLC() : Agent(StrategyTypes::ALLC) { complexityCost = StrategyComplexity::SIMPLE; }
        ResponseType decide(const ResponseType& lastResponse) override;

    };
}