#pragma once

#include "Agent.h"

/*
 *  The guidelines describe the rules of PAVLOV as:
 *      If the last outcome was CC (R) or DD (P) → stay with same choice.
 *      If the last outcome was CD (S) or DC (T) → switch choice next time.
 *  This would defeat the purpose of PAVLOV as it treats temptation payoffs as a lose state. Instead, I've used the
 *  following rules for the implementation:
 *      If the last outcome was CC (R) or DC (T) → stay with same choice.
 *      If the last outcome was CD (S) or DD (S) → switch choice next time.
 *  This distinguishes the PAVLOV strategy from the TFT strategy.
 */
namespace StrategyAgents {
    // Initial response: C
    class PAVLOV : public Agent {
    public:
        PAVLOV() : Agent(StrategyTypes::PAVLOV) { complexityCost = StrategyComplexity::MEDIUM; }

        void addToScore(double value) override;
        ResponseType decide(const ResponseType& lastResponse) override;
        void resetAgent() override;

    private:
        double lastPayoff{};
        ResponseType response = initialResponse;
    };
}