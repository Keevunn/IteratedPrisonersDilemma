#pragma once

#include "Agent.h"

namespace StrategyAgents {
    // Initial response: C
    class PAVLOV : public Agent {
    public:
        PAVLOV() {}
        explicit PAVLOV(Agent&& other) noexcept {}

        void addToScore(double value) override;
        ResponseType decideLogic(const ResponseType& lastResponse) override;

        friend std::ostream& operator<<(std::ostream& os, const PAVLOV& agent);

    private:
        double lastPayoff{};
        ResponseType response = initialResponse;
    };
}