#include "../../include/Output/CSV.h"

#include <ranges>
#include <sstream>

#include "../../include/MathUtil.h"
#include "../../include/Output/StatisticsUtil.h"

namespace Results::CSV {

    // Uncomment for payoff matrix output in csv file
    // std::ostream & Output::logResults(std::ostream &os) {
    //     os << "Strategy,Opponent,Mean,CI_lower,CI_upper,Rounds,Repeats,Seed,Epsilon,payoffs" << std::endl;
    //     for (const auto& match : tournament->getMatchResults())
    //         os << match;
    //     return os;
    // }
    std::ostream & Output::logResults(std::ostream &os) {
        // Metadata
        std::ostringstream metadata;
        {
            const std::string payoffs = "\"" + std::to_string(GameConfig::GameConfig::payoffs[0]) + "," +
                                        std::to_string(GameConfig::GameConfig::payoffs[1]) + "," +
                                        std::to_string(GameConfig::GameConfig::payoffs[2]) + "," +
                                        std::to_string(GameConfig::GameConfig::payoffs[3]) + "\"";
            metadata << GameConfig::GameConfig::rounds << "," << GameConfig::GameConfig::repeats << ","
                    << GameConfig::GameConfig::seed << "," << GameConfig::GameConfig::epsilon << "," << payoffs;
        }

        os << "Strategy,Mean,CI_lower,CI_upper,Rounds,Repeats,Seed,Epsilon,payoffs" << std::endl;
        auto stats = Statistics::generateStats(tournament->getTotalScores());
        for (const auto& strategy : stats) {
            os << strategy.name << "," << strategy.mean << ","  << strategy.CI.first << "," << strategy.CI.second << ","
            << metadata.str() << std::endl;
        }
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
                const std::string SCBval = ((useSCB) ? std::to_string(1) : std::to_string(0));
                metadata << population << "," << generations << "," << mutation << "," << SCBval << ","
                        << GameConfig::GameConfig::rounds << "," << GameConfig::GameConfig::repeats << ","
                        << GameConfig::GameConfig::seed << "," << GameConfig::GameConfig::epsilon << "," << payoffs;
            }

            // Header
            os << "Generation,Strategy,Share,Mean,CI_lower,CI_upper,Population,Total_Generations,Mutation,SCB,Rounds,Repeats,Seed,Epsilon,payoffs" << std::endl;

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
