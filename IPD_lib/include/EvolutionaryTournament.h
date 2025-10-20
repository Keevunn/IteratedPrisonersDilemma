#pragma once
#include "Tournament.h"

namespace Tournament::Evolution {

    class EvolutionaryTournament : public Tournament {
    public:
        EvolutionaryTournament(const std::vector<StrategyTypes>& strategies, const int population, const int generations, const double mutations) :
            Tournament(strategies), population(population), generations(generations), mutation(mutations) {
            initialise();
        };

        void simulateTournament() override;
        std::unordered_map<StrategyTypes, std::vector<double>>& getProportionHistory();
        std::unordered_map<StrategyTypes, std::vector<double>>& getAvgFitnessHistory();

    private:
        void runTournamentMatches();
        void initialise(); // Initialises strategyProportion, strategyFitness, proportionHistory
        void calculateExpectedFitness();
        [[nodiscard]] double calculateAverageFitness() const;
        void updateProportions();
        void resetFitness();
        void mutatePopulation();
        void updateHistory();

        int population; int generations; double mutation;
        std::unordered_map<StrategyTypes, double> strategyProportion;
        std::unordered_map<StrategyTypes, double> strategyFitness;
        std::unordered_map<StrategyTypes, std::vector<double>> proportionHistory; // For analysis
        std::unordered_map<StrategyTypes, std::vector<double>> avgFitnessHistory; // For analysis
    };
}
