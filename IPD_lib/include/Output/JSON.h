#pragma once
#include "Results.h"
#include "StatisticsUtil.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;
using namespace Statistics;

namespace Results::JSON {
    class Output : public ::Results::Output {
    public:
        explicit Output(std::unique_ptr<Tournament::Tournament>&& tournament) : ::Results::Output(std::move(tournament)) {}

        std::ostream& logResults(std::ostream& os) override;

        friend std::ostream& operator<<(std::ostream& os, const std::vector<OverallStats>& leaderboard);
        friend std::ostream& operator<<(std::ostream& os, const std::unordered_map<StrategyTypes, std::vector<double>>& payoffMatrix);

    private:
        void pushLeaderboard(json& obj, const std::vector<OverallStats>& leaderboard);
        void pushPayoffMatrix(json& obj, const std::unordered_map<StrategyTypes, std::vector<double>>& payoffMatrix);
    };

    namespace Evolution {
        class Output : public Results::Evolution::Output {
        public:
             Output(std::unique_ptr<Tournament::Evolution::EvolutionaryTournament>&& tournament, const int population, const int generations, const double mutation, const bool useSCB) :
                Results::Evolution::Output(std::move(tournament), population, generations, mutation, useSCB) {}

            std::ostream& logResults(std::ostream& os) override;

        };
    }
}