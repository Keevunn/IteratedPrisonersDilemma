#pragma once
#include <array>
#include <iostream>

namespace GameConfig {

    namespace Args{

         enum class ArgTypes{
            ROUNDS, REPEATS, SEED, EPSILON, PAYOFFS,
            STRATEGIES, FORMAT, SAVE, LOAD,
            EVOLVE, POPULATION, GENERATIONS, MUTATION
        };

    }

    namespace Strategies {

        enum class StrategyTypes{
            ALLC,ALLD,TFT,GRIM,PAVLOV,RND03,CONTRITE,PROBER
        };

    }

    // To avoid any errors with strings
    namespace Responses {
        enum class ResponseType{ C, D, INVALID };

        inline std::ostream& operator<<(std::ostream& os, const ResponseType& response) {
            return os << (response == ResponseType::C ? 'C' : 'D');
        }
    }

    // Inline static - single definition across TUs, all initialised to default values
    class GameConfig {
    public:
        inline static int rounds = 100;
        inline static int repeats = 1;
        inline static int seed{};
        inline static double epsilon{};
        inline static std::array<double, 4> payoffs = {5, 3, 1, 0};
        inline static std::string format = "text";
    };


}
