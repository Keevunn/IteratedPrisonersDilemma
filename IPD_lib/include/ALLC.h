#pragma once

#include "Agent.h"

namespace StrategyAgents {
    // Initial response: C
    class ALLC : public Agent {
    public:
        explicit ALLC(std::string  init) : Agent("C") {}
        ALLC(Agent&& other) noexcept : Agent("C") {}

        std::string decide(const std::string_view& lastResponse) override;
        std::string decide() override;

        friend std::ostream& operator<<(std::ostream& os, const Agent& agent);

    };
}