#pragma once
#include "../../include/GameConfig.h"
#include "../../include/Tournament.h"
#include "../../include/EvolutionaryTournament.h"

#include <iosfwd>
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

    [[nodiscard]] static std::vector<OverallStats> generateStats(const std::unordered_map<StrategyTypes, std::vector<double>>& data) ;
    [[nodiscard]] std::vector<OverallStats> generateLeaderboard(const std::unique_ptr<Tournament::Tournament>& tournament);
    [[nodiscard]] std::unordered_map<StrategyTypes, std::vector<double>> generatePayoffMatrix(const std::unique_ptr<Tournament::Tournament>& tournament);
    [[nodiscard]] std::vector<OverallStats> generateFitnessStats(const std::unique_ptr<Tournament::Evolution::EvolutionaryTournament>& tournament);

}
