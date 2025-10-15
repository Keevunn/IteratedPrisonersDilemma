#pragma once

#include "Agent.h"

namespace StrategyAgents {
    // Initial response: C
    class TFT : public Agent {
    public:
        TFT() : Agent() {}
        explicit TFT(Agent&& other) noexcept : Agent() {}

        friend std::ostream& operator<<(std::ostream& os, const TFT& agent);

    protected:
        ResponseType decideLogic(const ResponseType& lastResponse) override;

    };
}