#include "../../include/Output/JSON.h"

namespace Output::JSON {
    std::ostream& operator<<(std::ostream& os, const std::vector<Statistics::OverallStats>& leaderboard) {
        json output;
        output["leaderboard"] = json::array();
        for (const auto& stats : leaderboard) {
            json entry;
            entry["strategy"] = stats.name;
            entry["mean"] = stats.mean;
            entry["stdev"] = stats.stdDev;
            entry["CI"] = stats.CI;
            output["leaderboard"].push_back(entry);
        }
        return os << output;
    }

    std::ostream& operator<<(std::ostream& os, const std::unordered_map<StrategyTypes, std::vector<double>>& payoffMatrix) {
        json output;
        std::vector<StrategyTypes> strategies;
        for (const StrategyTypes& strat : payoffMatrix | std::views::keys) {
            strategies.push_back(strat);
        }

        for (const auto& [p1strat, dataRow] : payoffMatrix) {
            json entry;
            int col = 0;

            for (const auto& p1Mean : dataRow) {
                StrategyTypes p2strat = strategies[col++];
                entry[strategyToString(p1strat)][strategyToString(p2strat)] = p1Mean;
            }
            output["payoffMatrix"].push_back(entry);
        }
        return os << output;
    }

    std::string to_string( const std::ostream& os ) {
        std::ostringstream ss;
        ss << os.rdbuf();
        return ss.str();
    }

    std::ostream &JSON::results(std::ostream &os) {
        json output;
        output["metadata"]["rounds"] = GameConfig::GameConfig::rounds;
        output["metadata"]["repeats"] = GameConfig::GameConfig::repeats;
        output["metadata"]["seed"] = GameConfig::GameConfig::seed;
        output["metadata"]["epsilon"] = GameConfig::GameConfig::epsilon;
        output["metadata"]["payoffs"]["T"] = GameConfig::GameConfig::payoffs[0];
        output["metadata"]["payoffs"]["R"] = GameConfig::GameConfig::payoffs[1];
        output["metadata"]["payoffs"]["P"] = GameConfig::GameConfig::payoffs[2];
        output["metadata"]["payoffs"]["S"] = GameConfig::GameConfig::payoffs[3];

        pushLeaderboard(output, generateLeaderboard());

        pushPayoffMatrix(output, generatePayoffMatrix());

        return os << std::setw(1) << output;
    }

    void JSON::pushLeaderboard(json& obj, const std::vector<Statistics::OverallStats> &leaderboard) {
        obj["leaderboard"] = json::array();
        for (const auto& stats : leaderboard) {
            json entry;
            entry["strategy"] = stats.name;
            entry["mean"] = stats.mean;
            entry["stdev"] = stats.stdDev;
            entry["CI"] = stats.CI;
            obj["leaderboard"].push_back(entry);
        }
    }

    void JSON::pushPayoffMatrix(json &obj, const std::unordered_map<StrategyTypes, std::vector<double>>& payoffMatrix) {
        std::vector<StrategyTypes> strategies;
        for (const StrategyTypes& strat : payoffMatrix | std::views::keys) {
            strategies.push_back(strat);
        }

        for (const auto& [p1strat, dataRow] : payoffMatrix) {
            json entry;
            int col = 0;

            for (const auto& p1Mean : dataRow) {
                StrategyTypes p2strat = strategies[col++];
                entry[strategyToString(p1strat)][strategyToString(p2strat)] = p1Mean;
            }
            obj["payoffMatrix"].push_back(entry);
        }
    }
}
