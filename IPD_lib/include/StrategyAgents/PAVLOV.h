#pragma once

#include "Agent.h"

namespace StrategyAgents {
    // Initial response: C
    class PAVLOV : public Agent {
    public:
        PAVLOV() : Agent(StrategyTypes::PAVLOV) { complexityCost = StrategyComplexity::MEDIUM; }

        void addToScore(double value) override;
        ResponseType decide(const ResponseType& lastResponse) override;
        void resetAgent() override;

    private:
        double lastPayoff{};
        ResponseType response = initialResponse;
    };
}