#pragma once
#include "../../include/GameConfig.h"
#include "../../include/Tournament.h"
#include "../../include/EvolutionaryTournament.h"

#include <iosfwd>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

using namespace GameConfig::Strategies;

namespace Statistics {
    typedef struct OverallStatsStruct {
        OverallStatsStruct() = default;

        std::string name{};
        double mean{};
        std::pair<double, double> CI{};

        friend std::ostream& operator<<(std::ostream& os, const OverallStatsStruct& stats);
        friend std::ostream& operator<<(std::ostream& os, const std::vector<OverallStatsStruct>& rankedData);
    } OverallStats;

    [[nodiscard]] std::vector<OverallStats> generateStats(const std::unordered_map<StrategyTypes, std::vector<double>>& data) ;
    [[nodiscard]] std::vector<OverallStats> generateLeaderboard(const std::unique_ptr<Tournament::Tournament>& tournament);
    [[nodiscard]] std::vector<OverallStats> generateLeaderboard(std::vector<OverallStats> data);
    [[nodiscard]] std::pair<std::vector<StrategyTypes>,std::unordered_map<StrategyTypes, std::vector<double>>> // a pair of a vector storing the key order and the payoff matrix generated
                                        generatePayoffMatrix(const std::unique_ptr<Tournament::Tournament>& tournament);

}
