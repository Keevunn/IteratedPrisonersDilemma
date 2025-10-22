#pragma once
#include "Agent.h"

/* Firm But Fair (FBF):
 * FBF initially cooperates until betrayed, after which it retaliates proportionally. It's able to forgive the opponent
 * after a streak of cooperation but maintains a limited memory of past exploitation to avoid frequent S payoffs.
 *
 * Behavior:
 *      Round 1: Start with C
 *      Rule:
 *          Detects betrayal after 2 rounds of cooperating (choosing C)
 *          If betrayed, retaliates with N defections after Nth S payoff
 *          After punishment, returns to C to test if opponent has begun cooperating again
 *          Every 3 rounds of consecutive cooperation, N is reduced by 1 (clamped to 0)
 *      Note: The number of punishing rounds is capped at a certain value (e.g. 5) to allow the opponent to return to
 *            cooperation
 *
 * Retaliates after betrayal rather than a single exploitation round, to be more robust to noise
 * Recognises its own mistakes and tolerant of opponent's mistakes during cooperation phase
 */

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
        int coopCount = 1; // incremented when agent chooses C, capped at 2 for cooperation test
        int freezeResponse = 0;

        static constexpr int BASE_RETALIATION = 2;
        static constexpr int MAX_RETALIATION = 5;
        static constexpr int FORGIVENESS_THRESHOLD = 3;
        static constexpr int MAX_FREEZE = 3;
    };
}