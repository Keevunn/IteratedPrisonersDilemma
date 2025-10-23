#include "../../include/Output/StatisticsUtil.h"

#include <algorithm>

#include "../../include/MathUtil.h"

#include <iomanip>

namespace Statistics {

     std::ostream& operator<<(std::ostream& os, const OverallStatsStruct& stats) {
        os << std::left << std::setprecision(4)
            << std::setw(15) << stats.name + ": "
            << stats.mean << ", 95% CI [" << stats.CI.first << ", " << stats.CI.second << "]";
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const std::vector<OverallStats>& rankedData){
         int i = 1;
         for (const auto& stats : rankedData) {
             os << std::left << std::setw(5) << std::to_string(i++) + "."
             << stats << std::endl;
         }
         return os;
     }

    std::vector<OverallStats> generateStats(const std::unordered_map<StrategyTypes, std::vector<double>>& data) {
        std::vector<OverallStats> stats;
        for (const auto& [strat, values] : data) {
            OverallStats entry{};
            const int rpts = static_cast<int>(values.size());
            entry.name = strategyToString(strat);
            entry.mean = MathUtil::calculateMean(values, rpts);
            const double stdDev = MathUtil::calculateStdDev(values, rpts, entry.mean);
            entry.CI = MathUtil::calculateCI(entry.mean, stdDev, rpts );
            stats.emplace_back(entry);
        }
        return stats;
    }

    std::vector<OverallStats> generateLeaderboard(const std::unique_ptr<Tournament::Tournament>& tournament) {
        auto leaderboard = generateStats(tournament->getTotalPayoffs());
        std::ranges::sort(leaderboard,
                          [](const auto& a, const auto& b) { return a.mean > b.mean; });
        return leaderboard;
    }

    std::vector<OverallStats> generateLeaderboard(std::vector<OverallStats> data) {
         std::ranges::sort(data,
                          [](const auto& a, const auto& b) { return a.mean > b.mean; });
         return data;
    }

    std::map<StrategyTypes, std::vector<double>> generatePayoffMatrix(const std::unique_ptr<Tournament::Tournament>& tournament) {
        std::map<StrategyTypes, std::vector<double>> payoffMatrix;
        const auto& matches = tournament->getMatchResults();
        const int rowLen = static_cast<int>(std::sqrt(matches.size())); // matches.size() = (num of strategies)^2
        int row = 0; int col = 0;

         // Relies on consistent ordering of keys
        for (const auto& match : matches) {
            if (col >= rowLen) {
                row += 1;
                col = 0;
            }

            StrategyTypes p1Strat = match.player1->getStrategy();
            StrategyTypes p2Strat = match.player2->getStrategy();
            if (!payoffMatrix.contains(p1Strat)) {
                payoffMatrix[p1Strat] = std::vector(rowLen, -1.0);
            }
            // if there's a value in there already (i.e. not 0) average the two
            payoffMatrix[p1Strat][col] = (payoffMatrix[p1Strat][col] >= 0)
                                            ? (payoffMatrix[p1Strat][col] + match.p1Mean) / 2
                                            : match.p1Mean;

            if (!payoffMatrix.contains(p2Strat)) {
                payoffMatrix[p2Strat] = std::vector(rowLen, -1.0);
            }
            // To add in the player 2 mean of the reverse match
            payoffMatrix[p2Strat][row] = (payoffMatrix[p2Strat][row] >= 0)
                                            ? (payoffMatrix[p2Strat][row] + match.p2Mean) / 2
                                            : match.p2Mean;

            col += 1;
        }
        return payoffMatrix;
    }

}
