#include "../../include/StrategyAgents/RNDp.h"

#include <random>

namespace StrategyAgents {

    ResponseType RNDp::decideLogic(const ResponseType& lastResponse) {
        if (probabilityC == 1) return ResponseType::C;
        if (probabilityC == 0) return ResponseType::D;
        std::uniform_real_distribution<double> distribution(0, 1);
        std::random_device rd;
        std::mt19937 gen(rd());
        double chance = distribution(gen);

        if (chance <= probabilityC) return ResponseType::C;
        return ResponseType::D;
    }

    std::ostream& operator<<(std::ostream& os, const RNDp& agent) {
        os << std::string("Random, probability of cooperating = ") << agent.probabilityC;
        return os;
    }


}