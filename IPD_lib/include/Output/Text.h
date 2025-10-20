#pragma once
#include "Output.h"

namespace Output::Text {
    class Text : public Output {
    public:
        explicit Text(std::unique_ptr<Tournament::Tournament>& tournament) : Output(tournament) {}

        std::ostream& results(std::ostream& os) override;

        friend std::ostream& operator<<(std::ostream& os, const std::vector<Statistics::OverallStats>& leaderboard);
        friend std::ostream& operator<<(std::ostream& os, const std::unordered_map<StrategyTypes, std::vector<double>>& payoffMatrix);
        friend std::ostream& operator<<(std::ostream& os, std::vector<double>& dataRow);

    };
}