#pragma once

#include "Agent.h"

namespace StrategyAgents {
    // Initial response: C
    class TFT : public Agent {
    public:
        TFT() : Agent(StrategyTypes::TFT) { complexityCost = StrategyComplexity::MEDIUM; }

        ResponseType decide(const ResponseType& lastResponse) override;

    };
}