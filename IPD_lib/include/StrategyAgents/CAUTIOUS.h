#pragma once
#include "Agent.h"

namespace StrategyAgents {

    class CAUTIOUS : public Agent {
    public:
        CAUTIOUS() : Agent(ResponseType::D,StrategyTypes::CAUTIOUS) { complexityCost = StrategyComplexity::COMPLEX; }

        void addToScore(double value) override;
        ResponseType decide(const ResponseType& lastResponse) override;
        void resetAgent() override;

    private:
        double lastPayoff{};
        ResponseType response = initialResponse;
        int consecutivePunishments = 0;
        int freezeResponseCount = MAX_FREEZE;
        bool isInitialRound = true;

        static constexpr int MAX_FREEZE = 2;
    };
}
