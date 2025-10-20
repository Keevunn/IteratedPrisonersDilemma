#pragma once
#include <string>

#include "Agent.h"


namespace StrategyAgents {
    // Initial response: C
    class RNDp : public Agent {
    public:
        RNDp() : Agent(StrategyTypes::RND03) {}
        // explicit RNDp(const double p) : Agent("RND"+std::to_string(p)), probabilityC(p) {} (can be used for extension of program)

        ResponseType decide(const ResponseType& lastResponse) override;

    private:
        double probabilityC = 0.3;

    };
}