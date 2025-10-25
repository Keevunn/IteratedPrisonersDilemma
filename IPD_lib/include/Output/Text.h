#pragma once
#include "Results.h"
#include "StatisticsUtil.h"
using namespace Statistics;

namespace Results::Text {
    class Output : public Results::Output {
    public:
        explicit Output(std::unique_ptr<Tournament::Tournament>&& tournament) : Results::Output(std::move(tournament)) {}

        std::ostream& logResults(std::ostream& os) override;

        friend std::ostream& operator<<(std::ostream& os, std::pair<std::vector<StrategyTypes>,std::unordered_map<StrategyTypes, std::vector<double>>>& payoffMatrixData);
        friend std::ostream& operator<<(std::ostream& os, const std::vector<double>& dataRow);

    };

    namespace Evolution {
        struct historicalDataOutConfig {
            historicalDataOutConfig(const std::unordered_map<StrategyTypes, std::vector<double>>& historicalData, const int generations, const int increment, const bool shouldClamp=false)
                : historicalData(historicalData), generations(generations), increment(increment), shouldClamp(shouldClamp) {}
            const std::unordered_map<StrategyTypes, std::vector<double>>& historicalData {};
            const int generations; const int increment;
            const bool shouldClamp = false;
        };

        class Output : public Results::Evolution::Output {
        public:
             Output(std::unique_ptr<Tournament::Evolution::EvolutionaryTournament>&& tournament, const int population, const int generations, const double mutation, const bool useSCB) :
                Results::Evolution::Output(std::move(tournament), population, generations, mutation, useSCB) {}

            std::ostream& logResults(std::ostream& os) override;

            friend std::ostream& operator<<(std::ostream& os, const historicalDataOutConfig& historicalDataStruct);

        private:
            std::ostream& outputProportionData(std::ostream& os) const;
            std::ostream& outputFitnessData(std::ostream& os) const;

        };
    }
}
