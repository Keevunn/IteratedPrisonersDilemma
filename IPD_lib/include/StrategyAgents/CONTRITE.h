#pragma once

#include "Agent.h"

namespace StrategyAgents {
    // Initial response: C
    class CONTRITE : public Agent {
    public:
        CONTRITE() : Agent(StrategyTypes::CONTRITE) { complexityCost = StrategyComplexity::COMPLEX; }

        ResponseType decide(const ResponseType& lastResponse) override;
        void resetAgent() override;
    private:
        bool shouldApologise = false;
        bool apologisingNextRound = false;
    };
}