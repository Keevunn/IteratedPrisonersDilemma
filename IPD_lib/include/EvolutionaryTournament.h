#pragma once
#include "Tournament.h"

namespace Tournament::Evolution {

    struct GenerationData {
        StrategyTypes strategy;
        double proportion{};
        double avgFitness{};
        std::pair<double,double> CI{};
    };

    class EvolutionaryTournament : public Tournament {
    public:
        EvolutionaryTournament(const std::vector<StrategyTypes>& strategies, const int population, const int generations, const double mutations) : Tournament(
            strategies), population(population), generations(generations), mutation(mutations) {
            initialise();
        };

        void simulateTournament() override;

        [[nodiscard]] const std::vector<std::vector<GenerationData>>& getHistory() const;
        [[nodiscard]] std::vector<GenerationData> getGenerationData(int generation) const;

        std::unordered_map<StrategyTypes, std::vector<double>> getProportionHistory();
        std::vector<double> getProportionHistory(StrategyTypes strat);
        std::unordered_map<StrategyTypes, std::vector<double>> getAvgFitnessHistory();
        std::vector<double> getAvgFitnessHistory(StrategyTypes strat);

    private:
        void runTournamentMatches();
        void initialise(); // Initialises strategyProportion, strategyFitness, proportionHistory
        void calculateExpectedFitness();
        [[nodiscard]] double calculateAverageFitness();
        void updateProportions();
        void resetCurrentGeneration();
        void mutatePopulation();
        void updateFitnessHistory(std::unordered_map<StrategyTypes, std::vector<double>> allFitnessHistory);
        void updateHistory();

        // Helper to get strategy index
        [[nodiscard]] size_t getStrategyIndex(StrategyTypes strat) const;

        int population; int generations; double mutation;
        std::vector<GenerationData> currentGeneration;
        std::unordered_map<StrategyTypes, double> currentStrategyProportion;
        std::unordered_map<StrategyTypes, double> currentStrategyFitness;

        std::vector<std::vector<GenerationData>> history; // [generation][strategy_index]
        std::unordered_map<StrategyTypes, size_t> strategyIndex; // For quick strategy lookup
    };
}
