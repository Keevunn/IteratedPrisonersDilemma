#pragma once
#include "Agent.h"

/* CAUTIOUS PAVLOV (CPAVLOV):
 * Takes from the PAVLOV strategy, in which the player changes their response in reaction to the payoff of the last round.
 * CPAVLOV is instead initially suspicious of the oppenent, but will attempt to reconcile. Ultimately, it seeks to cooperate
 * with the opponent.
 *
 * Behavior:
 *      Round 1: Start with D for 2 rounds
 *      Rule:
 *          If the last outcome was CC (R) or DD (P) -> stay with same choice
 *          If the last outcome was CD (S) or DC (T) -> switch choice next time
 *          After 3 rounds of consecutive punishment (DD) -> Attempt reconciliation (Choose C for next 2 rounds)
 *      Note: Chooses D for 2 rounds at the beginning to avoid deadlock against reactive agents
 *            Chooses C for next 2 rounds, to give opponent a chance to forgive and avoid deadlock against reactive agents
 *            (i.e. DC -> CD loop)
 *
 * Can gain an initial advantage against cooperative opponents
 * Forgiving against reactive agents, but must receive a string of negative payoffs (P, S) first, sometimes receiving T
 * Receives sucker payoff (CD) every 3rd and 5th round against exploitative agents
 *
 * Noise behaviour:
 * More resistant to noise than PAVLOV
 */

namespace StrategyAgents {

    class CAUTIOUS : public Agent {
    public:
        CAUTIOUS() : Agent(ResponseType::D,StrategyTypes::CAUTIOUS) { complexityCost = StrategyComplexity::MEDIUM; }

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
