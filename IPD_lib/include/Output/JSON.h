#pragma once
#include "Output.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace Output::JSON {
    class JSON : public Output {
    public:
        explicit JSON(std::unique_ptr<Tournament::Tournament>& tournament) : Output(tournament) {}

        std::ostream& results(std::ostream& os) override;

        friend std::ostream& operator<<(std::ostream& os, const std::vector<Statistics::OverallStats>& leaderboard);
        friend std::ostream& operator<<(std::ostream& os, const std::unordered_map<StrategyTypes, std::vector<double>>& payoffMatrix);

    private:
        void pushLeaderboard(json& obj, const std::vector<Statistics::OverallStats>& leaderboard);
        void pushPayoffMatrix(json& obj, const std::unordered_map<StrategyTypes, std::vector<double>>& payoffMatrix);
    };
}