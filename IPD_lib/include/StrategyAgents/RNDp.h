#pragma once
#include "Agent.h"


namespace StrategyAgents {
    // Initial response: C
    class RNDp : public Agent {
    public:
        RNDp() : Agent() {}
        explicit RNDp(const double p) : Agent(), probabilityC(p) {}
        explicit RNDp(Agent&& other) noexcept : Agent() {}

        ResponseType decideLogic(const ResponseType& lastResponse) override;

        friend std::ostream& operator<<(std::ostream& os, const RNDp& agent);

    private:
        double probabilityC = 0.3;

    };
}