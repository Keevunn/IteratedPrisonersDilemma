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

    namespace Evolution {
        class Evolution : public Text {
        public:
            explicit Evolution(std::unique_ptr<Tournament::Tournament>& tournament, const int population, const int generations, const int mutation) :
                Text(tournament), population(population), generations(generations), mutation(mutation) {}

            std::ostream& results(std::ostream& os) override;

            friend std::ostream& operator<<(std::ostream& os, const std::vector<double>& row);
            friend std::ostream& operator<<(std::ostream& os, const std::unordered_map<StrategyTypes, std::vector<double>>& dataRows);

        private:
            int population; int generations; double mutation;
        };
    }
}