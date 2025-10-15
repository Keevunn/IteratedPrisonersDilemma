#pragma once

#include "Agent.h"

namespace StrategyAgents {
    // Initial response: C
    class ALLC : public Agent {
    public:
        ALLC() : Agent() {}
        explicit ALLC(Agent&& other) noexcept : Agent() {}

        friend std::ostream& operator<<(std::ostream& os, const ALLC& agent);

    protected:
        ResponseType decideLogic(const ResponseType& lastResponse) override;

    };
}