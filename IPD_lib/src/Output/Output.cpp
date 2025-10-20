#include "../../include/Output/Output.h"

#include <algorithm>
#include <iomanip>

#include "../../include/MathUtil.h"

namespace Output {

    namespace Statistics {
        std::ostream& operator<<(std::ostream& os, const OverallStats& stats) {
            os << std::left
                << std::setw(defaultWidth) << stats.name
                << std::setprecision(defaultPrecision) << stats.mean << " ± " << stats.stdDev
                << " [" << stats.CI.first << "-" << stats.CI.second << "]";
            return os;
        }
    }

    // Should be unreachable
    std::ostream & Output::results(std::ostream &os) { return os << "No Format Given!"; }

    std::vector<Statistics::OverallStats> Output::generateLeaderboard() const {
        std::vector<Statistics::OverallStats> leaderboard;
        const auto& totalScores = tournament->getTotalScores();
        for (const auto& [strat, scores] : totalScores) {
            Statistics::OverallStats stats{};
            stats.name = strat;
            stats.mean = MathUtil::calculateMean(scores, scores.size());
            stats.stdDev = MathUtil::calculateStdDev(scores, scores.size(), stats.mean);
            stats.CI = MathUtil::calculateCI(stats.mean, stats.stdDev, scores.size() );
            leaderboard.emplace_back(stats);
        }

        std::ranges::sort(leaderboard,
                          [](const auto& a, const auto& b) { return a.mean > b.mean; });

        return leaderboard;
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

    std::ostream& operator<<(std::ostream& os, Output& output) { return output.results(os); }
}
