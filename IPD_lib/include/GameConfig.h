#pragma once
#include <array>

namespace GameConfig {

    namespace Args{

         static enum class ArgTypes{
            ROUNDS, REPEATS, SEED, EPSILON, PAYOFFS,
            STRATEGIES, FORMAT, SAVE, LOAD,
            EVOLVE, POPULATION, GENERATIONS, MUTATION
        };

    }

    namespace Strategies {

        static enum class StrategyTypes{
            ALLC,ALLD,TFT,GRIM,PAVLOV,RND03,CONTRITE,PROBER
        };

    }

    // Inline static - single definition across TUs, all initialised to default values
    inline static int rounds = 100;
    inline static int repeats = 1;
    inline static int seed{};
    inline static double epsilon{};
    inline static std::array<double, 4> payoffs = {5, 3, 1, 0};


}
