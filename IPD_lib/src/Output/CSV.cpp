#include "../../include/Output/CSV.h"

#include <ranges>
#include <sstream>

#include "../../include/MathUtil.h"

namespace Results::CSV {
    std::ostream & Output::logResults(std::ostream &os) {
        os << "Strategy,Opponent,Mean,CI_lower,CI_upper,Rounds,Repeats,Seed,Epsilon,payoffs" << std::endl;
        for (const auto& match : tournament->getMatchResults())
            os << match;
        return os;
    }

    namespace Evolution {
        std::ostream &Output::logResults(std::ostream &os) {
            const auto& history = tournament->getHistory();

            // Metadata
            std::ostringstream metadata;
            {
                const std::string payoffs = "\"" + std::to_string(GameConfig::GameConfig::payoffs[0]) + "," +
                                            std::to_string(GameConfig::GameConfig::payoffs[1]) + "," +
                                            std::to_string(GameConfig::GameConfig::payoffs[2]) + "," +
                                            std::to_string(GameConfig::GameConfig::payoffs[3]) + "\"";
                metadata << population << "," << generations << "," << mutation << "," << GameConfig::GameConfig::rounds << ","
                        << GameConfig::GameConfig::repeats << "," << GameConfig::GameConfig::seed << ","
                        << GameConfig::GameConfig::epsilon << "," << payoffs;
            }

            // Header
            os << "Generation,Strategy,Share,Mean,CI_lower,CI_upper,Population,Total_Generations,Mutation,Rounds,Repeats,Seed,Epsilon,payoffs" << std::endl;

            // Body
            for (int i{}; i < generations ; ++i) {
                for (const auto&[strategy, proportion, avgFitness, CI]
                    : history[i]) {
                    os << i << "," << strategyToString(strategy) << "," << proportion << "," << avgFitness << ","
                        << CI.first << "," << CI.second << "," << metadata.str() << std::endl;
                }
            }

            return os;
        }
    }
}
