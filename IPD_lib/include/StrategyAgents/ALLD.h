#pragma once

#include "Agent.h"

namespace StrategyAgents {
    // Initial response: D
    class ALLD : public Agent {
    public:
        ALLD() : Agent(ResponseType::D) {}
        explicit ALLD(Agent&& other) noexcept : Agent(ResponseType::D) {} // Move constructor, always sets response to D

        friend std::ostream& operator<<(std::ostream& os, const ALLD& agent);

    protected:
        ResponseType decideLogic(const ResponseType& lastResponse) override;

    };
}