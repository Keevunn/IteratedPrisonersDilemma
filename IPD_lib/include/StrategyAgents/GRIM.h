#pragma once

#include "Agent.h"

namespace StrategyAgents {
    // Initial response: C
    class GRIM : public Agent {
    public:
        GRIM() : Agent("GRIM") {}

        ResponseType decide(const ResponseType& lastResponse) override;

        void resetAgent() override;

    private:
        bool playerHasDefected = false;

    };
}