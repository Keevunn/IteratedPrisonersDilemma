#pragma once

#include "Agent.h"

namespace StrategyAgents {
    // Initial response: D
    class ALLD : public Agent {
    public:
        ALLD() : Agent(ResponseType::D, StrategyTypes::ALLD) { complexityCost = StrategyComplexity::SIMPLE; }

        ResponseType decide(const ResponseType& lastResponse) override;

    };
}