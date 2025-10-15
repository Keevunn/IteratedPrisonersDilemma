#pragma once

#include "Agent.h"

namespace StrategyAgents {
    // Initial response (Probe phase): C, D, C, C
    class PROBER : public Agent {
    public:
        PROBER() : Agent() {}
        explicit PROBER(Agent&& other) noexcept : Agent() {}

        void addToScore(double value) override;
        ResponseType decideLogic(const ResponseType& lastResponse) override;

        friend std::ostream& operator<<(std::ostream& os, const PROBER& agent);

    private:
        std::array<ResponseType, 4> probePhase = {ResponseType::C, ResponseType::D, ResponseType::C, ResponseType::C};
        int currentRound = 0;
        double lastPayoff{};
        bool isProbing = true; bool canExploit = false;

    };
}