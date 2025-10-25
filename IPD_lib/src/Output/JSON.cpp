#include "../../include/Output/JSON.h"

namespace Results::JSON {
    std::ostream& operator<<(std::ostream& os, const std::vector<Statistics::OverallStats>& leaderboard) {
        json output;
        output["leaderboard"] = json::array();
        for (const auto& stats : leaderboard) {
            json entry;
            entry["strategy"] = stats.name;
            entry["mean"] = stats.mean;
            entry["CI"] = stats.CI;
            output["leaderboard"].push_back(entry);
        }
        return os << output;
    }

    std::ostream& operator<<(std::ostream& os, const std::map<StrategyTypes, std::vector<double>>& payoffMatrix) {
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

    std::ostream &Output::logResults(std::ostream &os) {
        json output;
        output["metadata"]["rounds"] = GameConfig::GameConfig::rounds;
        output["metadata"]["repeats"] = GameConfig::GameConfig::repeats;
        output["metadata"]["seed"] = GameConfig::GameConfig::seed;
        output["metadata"]["epsilon"] = GameConfig::GameConfig::epsilon;
        output["metadata"]["payoffs"] = json::array({GameConfig::GameConfig::payoffs[0], GameConfig::GameConfig::payoffs[1], GameConfig::GameConfig::payoffs[2], GameConfig::GameConfig::payoffs[3]});

        pushLeaderboard(output, generateLeaderboard(tournament));

        pushPayoffMatrix(output, generatePayoffMatrix(tournament).second);

        return os << std::setw(2) << output;
    }

    void Output::pushLeaderboard(json& obj, const std::vector<OverallStats> &leaderboard) {
        obj["leaderboard"] = json::array();
        for (const auto& stats : leaderboard) {
            json entry;
            entry["strategy"] = stats.name;
            entry["mean"] = stats.mean;
            entry["CI"] = stats.CI;
            obj["leaderboard"].push_back(entry);
        }
    }

    void Output::pushPayoffMatrix(json &obj, const std::unordered_map<StrategyTypes, std::vector<double>>& payoffMatrix) {
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

    namespace Evolution {
        std::ostream& Output::logResults(std::ostream &os) {
            json output;
            // Metadata
            output["metadata"]["population"] = population;
            output["metadata"]["generations"] = generations;
            output["metadata"]["mutation"] = mutation;
            output["metadata"]["rounds"] = GameConfig::GameConfig::rounds;
            output["metadata"]["repeats"] = GameConfig::GameConfig::repeats;
            output["metadata"]["seed"] = GameConfig::GameConfig::seed;
            output["metadata"]["epsilon"] = GameConfig::GameConfig::epsilon;
            output["metadata"]["payoffs"] = json::array({GameConfig::GameConfig::payoffs[0], GameConfig::GameConfig::payoffs[1], GameConfig::GameConfig::payoffs[2], GameConfig::GameConfig::payoffs[3]});
            output["metadata"]["scb"] = useSCB;

            // Results - Array of json objects, each for a single observation
            output["results"] = json::array();

            const auto& history = tournament->getHistory();

            for (int i{}; i < generations ; ++i) {
                json gen;
                gen["generation"] = i;
                gen["strategies"] = json::array(); // an array of json objects, each for a single strategy

                for (const auto&[strategy, proportion, avgFitness, CI] : history[i]) {
                    json strat;
                    strat["strategy"] = strategyToString(strategy);
                    strat["share"] = proportion;
                    strat["mean"] = avgFitness;
                    strat["CI"] = CI;
                    gen["strategies"].push_back(strat);
                }
                output["results"].push_back(gen);
            }

            os << std::setw(2) << output;
            return os;
        }
    }
}
