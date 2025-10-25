#pragma once
#include "Agent.h"

namespace StrategyAgents {

    class FBF : public Agent {
    public:
        FBF() : Agent(StrategyTypes::FBF) { complexityCost = StrategyComplexity::COMPLEX; }

        void addToScore(double value) override;
        ResponseType decide(const ResponseType& lastResponse) override;
        void resetAgent() override;

    private:
        double lastPayoff{};
        ResponseType response = initialResponse;
        int remainingRetaliation = 0;
        int exploitationCount = 0;
        int mutualCooperationStreak = 0;
        int apologisePhase = 0;
        bool opponentCanCooperate = false;

        static constexpr int BASE_RETALIATION = 1;
        static constexpr int MAX_RETALIATION = 5;
        static constexpr int FORGIVENESS_THRESHOLD = 3;
        static constexpr int MAX_FREEZE = 2;
    };
}