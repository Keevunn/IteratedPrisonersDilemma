#pragma once

#include "Agent.h"

namespace StrategyAgents {
    // Initial response: D
    class ALLD : public Agent {
    public:
        explicit ALLD(std::string  init) : Agent("D") {} // No matter what's given initial response is always D
        ALLD(Agent&& other) noexcept : Agent("D") {}

        std::string decide(const std::string_view& lastResponse) override;
        std::string decide() override;

        friend std::ostream& operator<<(std::ostream& os, const Agent& agent);

    };
}