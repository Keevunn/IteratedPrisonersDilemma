#include "../../include/Output/Text.h"

#include <iomanip>
#include <ranges>

namespace Output::Text {
    std::ostream& operator<<(std::ostream& os, const std::vector<Statistics::OverallStats>& leaderboard){
        int i = 1;
        for (const auto& stats : leaderboard) {
            os << std::left << i++
            << std::setw(5) << "."
            << stats << std::endl;
        }
        return os << std::endl;
    }

    std::ostream& operator<<(std::ostream& os, const std::vector<double>& dataRow) {
        for (const auto& entry : dataRow)
            os << std::setw(defaultWidth) << std::setprecision(defaultPrecision) << entry;

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

    std::ostream &Text::results(std::ostream &os) {
        os  << " Tournament Summary " << std::endl
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
            << "Mean ± stdev [95% CI]" << std::endl;

        os  << generateLeaderboard() << std::endl;

        os  << " Payoff Matrix " << std::endl
            << divider << std::endl;

        os  << generatePayoffMatrix();

        return os;

    }





}
