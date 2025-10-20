#pragma once
#include "Results.h"

namespace Results::Text {
    class Output : public ::Results::Output {
    public:
        explicit Output(std::unique_ptr<Tournament::Tournament>&& tournament) : ::Results::Output(std::move(tournament)) {}

        std::ostream& logResults(std::ostream& os) override;

        friend std::ostream& operator<<(std::ostream& os, const std::vector<Statistics::OverallStats>& rankedData);
        friend std::ostream& operator<<(std::ostream& os, const std::unordered_map<StrategyTypes, std::vector<double>>& payoffMatrix);
        friend std::ostream& operator<<(std::ostream& os, std::vector<double>& dataRow);

    private:
        std::vector<Statistics::OverallStats> generateLeaderboard() const;

    };

    namespace Evolution {
        struct historicalDataOutConfig {
            const std::unordered_map<StrategyTypes, std::vector<double>>& historicalData;
            const int generations; const int increment;
        };

        class Output : public Text::Output {
        public:
             Output(std::unique_ptr<Tournament::Evolution::EvolutionaryTournament>&& tournament, const int population, const int generations, const int mutation) :
                Text::Output(std::move(tournament)), population(population), generations(generations), mutation(mutation) {}

            std::ostream& logResults(std::ostream& os) override;

            friend std::ostream& operator<<(std::ostream& os, const historicalDataOutConfig& historicalDataStruct);

        protected:
            std::unique_ptr<Tournament::Evolution::EvolutionaryTournament> tournament; // TODO Need to test if ptr moved correctly

        private:
            std::ostream& outputProportionData(std::ostream& os) const;
            std::ostream& outputFitnessData(std::ostream& os) const;

            [[nodiscard]] std::vector<Statistics::OverallStats> generateFitnessStats() const;

            int population; int generations; double mutation;

        };
    }
}
