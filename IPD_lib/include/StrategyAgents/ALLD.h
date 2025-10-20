#pragma once

#include "Agent.h"

namespace StrategyAgents {
    // Initial response: D
    class ALLD : public Agent {
    public:
        ALLD() : Agent(ResponseType::D, StrategyTypes::ALLD) {}

    protected:
        ResponseType decide(const ResponseType& lastResponse) override;

    };
}