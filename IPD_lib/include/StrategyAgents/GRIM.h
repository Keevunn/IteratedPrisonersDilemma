#pragma once

#include "Agent.h"

namespace StrategyAgents {
    // Initial response: C
    class GRIM : public Agent {
    public:
        GRIM() : Agent() {}
        explicit GRIM(Agent&& other) noexcept : Agent() {}

        friend std::ostream& operator<<(std::ostream& os, const GRIM& agent);

    protected:
        ResponseType decideLogic(const ResponseType& lastResponse) override;

    private:
        bool playerHasDefected = false;

    };
}