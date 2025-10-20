#pragma once
#include "Results.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace Results::JSON {
    class JSON : public Output {
    public:
        explicit JSON(std::unique_ptr<Tournament::Tournament>& tournament) : Output(tournament) {}

        std::ostream& logResults(std::ostream& os) override;

        friend std::ostream& operator<<(std::ostream& os, const std::vector<Statistics::OverallStats>& leaderboard);
        friend std::ostream& operator<<(std::ostream& os, const std::unordered_map<StrategyTypes, std::vector<double>>& payoffMatrix);

    private:
        void pushLeaderboard(json& obj, const std::vector<Statistics::OverallStats>& leaderboard);
        void pushPayoffMatrix(json& obj, const std::unordered_map<StrategyTypes, std::vector<double>>& payoffMatrix);
    };

    namespace Evolution {
        class Evolution : public JSON {
        public:
            explicit Evolution(std::unique_ptr<Tournament::Tournament>& tournament, const int population, const int generations, const int mutation) :
                JSON(tournament), population(population), generations(generations), mutation(mutation) {}

            std::ostream& logResults(std::ostream& os) override;

        private:
            int population; int generations; double mutation;
        };
    }
}