#include "../include/Agent.h"

namespace StrategyAgents {

    class Strategies : public Agent {
    public:
        std::string decide(const std::string_view &lastResponse) override { return "C"; }
    };
}