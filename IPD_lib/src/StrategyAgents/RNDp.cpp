#include "../../include/StrategyAgents/RNDp.h"

#include <random>

namespace StrategyAgents {

    ResponseType RNDp::decide(const ResponseType& lastResponse) {
        if (probabilityC == 1) return ResponseType::C;
        if (probabilityC == 0) return ResponseType::D;
        std::uniform_real_distribution<double> distribution(0, 1);
        //std::random_device rd;
        std::mt19937 gen(GameConfig::GameConfig::seed);

        if ( distribution(gen) <= probabilityC) return ResponseType::C;
        return ResponseType::D;
    }

}