#include "../../include/Output/Results.h"

#include <iomanip>

#include "../../include/MathUtil.h"

namespace Results {

    namespace Statistics {
        std::ostream& operator<<(std::ostream& os, const OverallStats& stats) {
            os << std::left << std::setprecision(defaultPrecision)
                << std::setw(defaultWidth) << stats.name
                << stats.mean << " ± " << stats.stdDev
                << " [" << stats.CI.first << "-" << stats.CI.second << "]";
            return os;
        }
    }

    // Should be unreachable
    std::ostream & Output::logResults(std::ostream &os) { return os << "No Format Given!"; }

    std::vector<Statistics::OverallStats> Output::generateStats(const std::unordered_map<StrategyTypes, std::vector<double>>& data) {
        std::vector<Statistics::OverallStats> stats;
        for (const auto& [strat, values] : data) {
            Statistics::OverallStats entry{};
            const int rpts = static_cast<int>(values.size());
            entry.name = strategyToString(strat);
            entry.mean = MathUtil::calculateMean(values, rpts);
            entry.stdDev = MathUtil::calculateStdDev(values, rpts, entry.mean);
            entry.CI = MathUtil::calculateCI(entry.mean, entry.stdDev, rpts );
            stats.emplace_back(entry);
        }

        return stats;
    }

    std::unordered_map<StrategyTypes, std::vector<double>> Output::generatePayoffMatrix() const {
        std::unordered_map<StrategyTypes, std::vector<double>> payoffMatrix;
        const auto& matches = tournament->getMatchResults();
        const int rowLen = static_cast<int>(std::sqrt(matches.size())); // matches.size() = (num of strategies)^2
        int row = 0; int col = 0;

        for (const auto& match : matches) {
            if (col >= rowLen) {
                row += 1;
                col = 0;
            }

            StrategyTypes p1Strat = match.player1->getStrategy();
            StrategyTypes p2Strat = match.player2->getStrategy();
            if (!payoffMatrix.contains(p1Strat)) {
                payoffMatrix[p1Strat] = std::vector(rowLen, 0.0);
            }
            payoffMatrix[p1Strat][col] = match.p1Mean;

            if (!payoffMatrix.contains(p2Strat)) {
                payoffMatrix[p2Strat] = std::vector(rowLen, 0.0);
            }
            payoffMatrix[p2Strat][row] = (payoffMatrix[p2Strat][row] + match.p2Mean) / 2; // To add in the player 2 mean of the reverse match

            col += 1;
        }
        return payoffMatrix;
    }

    std::ostream& operator<<(std::ostream& os, Output& output) { return output.logResults(os); }
}
