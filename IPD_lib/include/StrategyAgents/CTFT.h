#pragma once

#include "Agent.h"

namespace StrategyAgents {
    // Initial response: C
    class CTFT : public Agent {
    public:
        CTFT() : Agent() {}
        explicit CTFT(Agent&& other) noexcept : Agent() {}

        std::string decideLogic(const std::string_view& lastResponse) override;

        friend std::ostream& operator<<(std::ostream& os, const Agent& agent);

    };
}