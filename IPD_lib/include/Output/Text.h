#pragma once
#include "Results.h"

namespace Results::Text {
    class Output : public Results::Output {
    public:
        explicit Output(std::unique_ptr<Tournament::Tournament>&& tournament) : ::Results::Output(std::move(tournament)) {}

        std::ostream& logResults(std::ostream& os) override;

        friend std::ostream& operator<<(std::ostream& os, const std::vector<Statistics::OverallStats>& rankedData);
        friend std::ostream& operator<<(std::ostream& os, const std::unordered_map<StrategyTypes, std::vector<double>>& payoffMatrix);
        friend std::ostream& operator<<(std::ostream& os, std::vector<double>& dataRow);

    };

    namespace Evolution {
        struct historicalDataOutConfig {
            const std::unordered_map<StrategyTypes, std::vector<double>>& historicalData;
            const int generations; const int increment;
        };

        class Output : public Results::Evolution::Output {
        public:
             Output(std::unique_ptr<Tournament::Evolution::EvolutionaryTournament>&& tournament, const int population, const int generations, const double mutation) :
                Results::Evolution::Output(std::move(tournament), population, generations, mutation) {}

            std::ostream& logResults(std::ostream& os) override;

            friend std::ostream& operator<<(std::ostream& os, const historicalDataOutConfig& historicalDataStruct);

        private:
            std::ostream& outputProportionData(std::ostream& os) const;
            std::ostream& outputFitnessData(std::ostream& os) const;

        };
    }
}
