#pragma once

#include "Agent.h"

namespace StrategyAgents {
    // Initial response (Probe phase): C, D, C, C
    class PROBER : public Agent {
    public:
        PROBER() : Agent(StrategyTypes::PROBER) {}

        void addToScore(double value) override;
        ResponseType decide(const ResponseType& lastResponse) override;
        void resetAgent() override;

    private:
        const std::array<ResponseType, 4> probePhase = {ResponseType::C, ResponseType::D, ResponseType::C, ResponseType::C};
        int currentRound = 0;
        double lastPayoff{};
        bool isProbing = true; bool canExploit = false;

    };
}