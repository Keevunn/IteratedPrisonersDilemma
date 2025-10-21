#include "../../include/Output/Text.h"
#include "../../include/Output/StatisticsUtil.h"

#include <iomanip>
#include <ranges>
#include <sstream>

using namespace Statistics;

namespace Results::Text {


    std::ostream& operator<<(std::ostream& os, const std::vector<double>& dataRow) {
        for (const auto& entry : dataRow)
            os << std::setprecision(defaultPrecision) << std::setw(defaultWidth) << entry;

        return os << std::endl;
    }

    std::ostream& operator<<(std::ostream& os, const std::unordered_map<StrategyTypes, std::vector<double>>& payoffMatrix) {
        // player 2 header row
        os << std::left << std::setw(defaultWidth) << " ";
        for (const StrategyTypes& strat : payoffMatrix | std::views::keys) {
            os  << std::setw(defaultWidth) << strategyToString(strat);
        }
        os << std::endl;

        for (const auto& [strat, dataRow] : payoffMatrix) {
            os << std::setw(defaultWidth) << strategyToString(strat) << dataRow;
        }

        return os << std::endl;
    }

    std::ostream& Output::logResults(std::ostream &os) {
        os  << std::left << std::setprecision(defaultPrecision)
            << " Tournament Summary " << std::endl
            << divider << std::endl
            << "Rounds: " << GameConfig::GameConfig::rounds
            << "\tRepeats: " << GameConfig::GameConfig::repeats
            << "\tSeed: " << GameConfig::GameConfig::seed
            << "\tEpsilon: " << GameConfig::GameConfig::epsilon << std::endl
            << "Payoffs: T=" << GameConfig::GameConfig::payoffs[0]
            << ", R=" << GameConfig::GameConfig::payoffs[1]
            << ", P=" << GameConfig::GameConfig::payoffs[2]
            << ", S=" << GameConfig::GameConfig::payoffs[3]
            << std::endl << std::endl;

        os  << " Leaderboard " << std::endl
            << divider << std::endl
            << std::left << std::setw(6) << " " << std::setw(defaultWidth) << "Strategy"
            << "Mean" << std::endl;

        os  << generateLeaderboard(tournament) << std::endl;

        os  << " Payoff Matrix " << std::endl
            << divider << std::endl;

        os  << generatePayoffMatrix(tournament);

        return os;

    }


    namespace Evolution {

        std::ostream& operator<<(std::ostream& os, const std::vector<double>& row) {
            for (const auto& entry : row)
                os << std::setw(defaultWidth) << entry;

            return os << std::endl;
        }

        std::ostream& operator<<(std::ostream& os, const historicalDataOutConfig& historicalDataStruct) {
            std::ostringstream header;
            header  << std::left << std::setw(defaultWidth) << "t =";

            std::ostringstream body;
            body << std::left << std::setprecision(defaultPrecision);

            bool isHeaderComplete = false;
            bool hasPrintedFinalCol = false;

            const auto& historicalData = historicalDataStruct.historicalData;
            const auto generations = historicalDataStruct.generations;
            const auto increment = historicalDataStruct.increment;

            for (const auto& [strat, data] : historicalData) {
                body << std::setw(defaultWidth) << strategyToString(strat);

                for (int pos{}; pos < generations ; pos+=increment) {
                    if (!isHeaderComplete) {
                        header << std::setw(defaultWidth) << pos;
                    }
                    body << std::setw(defaultWidth) << data[pos];
                    hasPrintedFinalCol = (pos == generations - 1);
                }
                if (!hasPrintedFinalCol)
                    body << data[generations - 1];

                // after the first complete iteration of the inner for loop, header will be complete
                if (!isHeaderComplete) {
                    if (!hasPrintedFinalCol) header << generations - 1;
                    header << std::endl;
                    isHeaderComplete = true;
                }

                body << std::endl;
            }
            os << header.str() << body.str();
            return os;
        }

        std::ostream& Output::logResults(std::ostream &os) {
            os  << std::left << std::setprecision(defaultPrecision)
            << " Tournament Summary " << std::endl
            << divider << std::endl
            << "Rounds: " << GameConfig::GameConfig::rounds
            << "\tRepeats: " << GameConfig::GameConfig::repeats
            << "\tSeed: " << GameConfig::GameConfig::seed
            << "\tEpsilon: " << GameConfig::GameConfig::epsilon << std::endl
            << "Payoffs: T=" << GameConfig::GameConfig::payoffs[0]
            << ", R=" << GameConfig::GameConfig::payoffs[1]
            << ", P=" << GameConfig::GameConfig::payoffs[2]
            << ", S=" << GameConfig::GameConfig::payoffs[3] << std::endl
            << "Population: " << population
            <<"\tGenerations: " << generations
            << "\tMutations: " << mutation
            << std::endl;

            outputProportionData(os);
            outputFitnessData(os);

            return os;
        }

        std::ostream& Output::outputProportionData(std::ostream& os) const {
            os  << std::endl << " Final Population Shares " << std::endl
                << divider << std::endl
                << "Total Population = " << population << std::endl;

            const auto& finalGenerationData = tournament->getGenerationData(generations-1);
            for (const auto& strategyValues : finalGenerationData) {
                // Final population shares
                // e.g. ALLC: 100 (10%)
                os  << std::setw(defaultWidth) << strategyToString(strategyValues.strategy) + ": "
                    << population * strategyValues.proportion << " (" << strategyValues.proportion*100.0 << "%)" << std::endl;
            }

            const auto& proportionHistory = tournament->getProportionHistory();
            constexpr int maxColumns = 10;
            const int increment = generations <= maxColumns ? 1 : generations / (maxColumns - 2); // Always includes gen 0 and last gen
            os << std::left << std::endl << " Population shares after each generation " << std::endl << divider << std::endl;
            os << historicalDataOutConfig(proportionHistory, generations, increment);

            return os;
        }

        std::ostream& Output::outputFitnessData(std::ostream& os) const {
            os  << std::endl << " Final Average Fitness " << std::endl
                << divider << std::endl;

            const auto& fitnessHistory = tournament->getAvgFitnessHistory();
            // Final Average fitness
            // e.g. ALLC: (mean), 95% CI [CI]
            os << generateFitnessStats(tournament) << std::endl; // explicit call to avoid redefining

            constexpr int maxColumns = 10;
            const int increment = generations <= maxColumns ? 1 : generations / (maxColumns - 1);
            os << std::left << std::endl << " Average fitness over time " << std::endl << divider << std::endl;
            os << historicalDataOutConfig(fitnessHistory, generations, increment);
            return os;
        }


    }

}
