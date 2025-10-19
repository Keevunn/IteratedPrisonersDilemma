#pragma once

#include "Agent.h"

namespace StrategyAgents {
    // Initial response: C
    class TFT : public Agent {
    public:
        TFT() : Agent("TFT") {}

    protected:
        ResponseType decide(const ResponseType& lastResponse) override;

    };
}