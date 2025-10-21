#include "../include/EvolutionaryTournament.h"

#include <numeric>
#include <ranges>
#include <string>

#include "../include/Match.h"
#include "../include/MathUtil.h"
#include "../include/RandomUtil.h"

namespace Tournament::Evolution {
    void EvolutionaryTournament::simulateTournament() {
        for (int i{}; i < generations; ++i) {
            runTournamentMatches();
            calculateExpectedFitness();
            updateProportions();
            mutatePopulation();
            updateHistory();
            resetCurrentGeneration();
        }
    }

    const std::vector<std::vector<GenerationData>> & EvolutionaryTournament::getHistory() const { return history; }

    std::vector<GenerationData> EvolutionaryTournament::getGenerationData(const int generation) const {
        if (generation >= 0 && generation < history.size()) {
            return history[generation];
        }
        return {};
    }

    std::unordered_map<StrategyTypes, std::vector<double>> EvolutionaryTournament::getProportionHistory() {
        std::unordered_map<StrategyTypes, std::vector<double>> proportionsHistory;
        for (const auto& strat : strategies) {
            proportionsHistory[strat] = getProportionHistory(strat);
        }
        return proportionsHistory;
    }

    std::vector<double> EvolutionaryTournament::getProportionHistory(const StrategyTypes strat) {
        std::vector<double> proportions;
        proportions.reserve(history.size());
        const size_t idx = getStrategyIndex(strat);

        for (const auto& generation : history)
            proportions.push_back(generation[idx].proportion);

        return proportions;
    }

    std::unordered_map<StrategyTypes, std::vector<double>> EvolutionaryTournament::getAvgFitnessHistory() {
        std::unordered_map<StrategyTypes, std::vector<double>> fitnessHistory;
        for (const auto& strat : strategies) {
            fitnessHistory[strat] = getAvgFitnessHistory(strat);
        }
        return fitnessHistory;
    }

    std::vector<double> EvolutionaryTournament::getAvgFitnessHistory(const StrategyTypes strat) {
        std::vector<double> fitness;
        fitness.reserve(history.size());
        const size_t idx = getStrategyIndex(strat);

        for (const auto& generation : history)
            fitness.push_back(generation[idx].avgFitness);

        return fitness;
    }

    void EvolutionaryTournament::runTournamentMatches() {
        for (auto& match : matchResults) {
            std::vector<double> p1MeanPayoff{};
            std::vector<double> p2MeanPayoff{};
            p1MeanPayoff.reserve(GameConfig::GameConfig::repeats);
            p2MeanPayoff.reserve(GameConfig::GameConfig::repeats);
            std::string key = match.player1->getName() + "," + match.player2->getName();

            for (int i{};  i < GameConfig::GameConfig::repeats; i++) { // Play each match "repeats" times
                Match::simulateMatch(match.player1, match.player2);
                double p1Mean = match.player1->getScore() / GameConfig::GameConfig::rounds;
                p1MeanPayoff.emplace_back(p1Mean);
                double p2Mean = match.player2->getScore() / GameConfig::GameConfig::rounds;
                p2MeanPayoff.emplace_back(p2Mean);
                // Reset score for each player so next match starts fresh
                match.player1->resetAgent();
                match.player2->resetAgent();
            }
            match.p1Mean = MathUtil::calculateMean(p1MeanPayoff, GameConfig::GameConfig::repeats);
            match.p2Mean = MathUtil::calculateMean(p2MeanPayoff, GameConfig::GameConfig::repeats);
        }
    }

    void EvolutionaryTournament::initialise() {
        double equalShare = 1.0 / strategies.size();
        for (int i{}; i < strategies.size(); ++i) {
            const auto& strat = strategies[i];
            strategyIndex[strat] = i;
            currentStrategyProportion[strat] = equalShare;
            currentStrategyFitness[strat] = 0.0;
        }

        history.reserve(generations);
        currentGeneration.resize(strategies.size());
    }

    // Returns expected fitness for the entire population
    void EvolutionaryTournament::calculateExpectedFitness() {
        std::unordered_map<StrategyTypes, std::vector<double>> allFitnessHistory;
        for (const auto& match : matchResults) {
            StrategyTypes p1Strat = match.player1->getStrategy();
            StrategyTypes p2Strat = match.player2->getStrategy();

            const double p1WeightedFitness = match.p1Mean * currentStrategyProportion[p2Strat];
            currentStrategyFitness[p1Strat] += p1WeightedFitness;
            allFitnessHistory[p1Strat].emplace_back(p1WeightedFitness);

            const double p2WeightedFitness = match.p1Mean * currentStrategyProportion[p1Strat];
            currentStrategyFitness[p2Strat] += p2WeightedFitness;
            allFitnessHistory[p2Strat].emplace_back(p2WeightedFitness);
        }
        updateFitnessHistory(allFitnessHistory);
    }

    double EvolutionaryTournament::calculateAverageFitness()  {
        double avg = 0;
        for (const StrategyTypes& strat : strategies) {
            avg += (currentStrategyFitness[strat] * currentStrategyProportion[strat]);
        }
        return avg;
    }

    void EvolutionaryTournament::updateProportions() {
        double avgFitness = calculateAverageFitness();
        for (auto& [strat, proportion] : currentStrategyProportion) {
            proportion = proportion * (currentStrategyFitness.at(strat) / avgFitness);
        }
    }

    void EvolutionaryTournament::resetCurrentGeneration() {
        for (const auto &strat: strategies)
            currentStrategyFitness.at(strat) = 0;

        currentGeneration.resize(strategies.size());
    }

    void EvolutionaryTournament::mutatePopulation() {
        for (auto& [strat, proportion] : currentStrategyProportion) {
            if (Random::Random::getRandomDouble() >= mutation) continue;

            double reduction = proportion * (Random::Random::getRandomDouble() * 0.02); // Reduces the proportion by a random value between 1-2%
            currentStrategyProportion[strat] -= reduction;

            // Redistribute the reduction to random other strategy
            StrategyTypes newStrat = strat;
            while (newStrat != strat)
                newStrat = strategies[static_cast<int>(Random::Random::getRandomDouble() * strategies.size())];
            currentStrategyProportion[newStrat] += reduction;
        }
    }

    void EvolutionaryTournament::updateFitnessHistory(std::unordered_map<StrategyTypes, std::vector<double>> allFitnessHistory) {
        for (const auto& strat : strategies) {
            const double stdDev = MathUtil::calculateStdDev(allFitnessHistory[strat], allFitnessHistory[strat].size(), currentStrategyFitness[strat]);

            const size_t idx = getStrategyIndex(strat);
            // Store in the current generation being built (will be added in updateHistory)
            currentGeneration[idx].avgFitness = currentStrategyFitness[strat];
            currentGeneration[idx].CI = MathUtil::calculateCI(currentGeneration[idx].avgFitness,
                stdDev, GameConfig::GameConfig::repeats);
        }
    }

    void EvolutionaryTournament::updateHistory() {

        for (const auto& strat : strategies) {
            size_t idx = getStrategyIndex(strat);
            currentGeneration[idx].strategy = strat;
            currentGeneration[idx].proportion = currentStrategyProportion[strat];
        }

        history.push_back(std::move(currentGeneration));

    }

    size_t EvolutionaryTournament::getStrategyIndex(const StrategyTypes strat) const {
        auto it = strategyIndex.find(strat);
        if (it != strategyIndex.end()) {
            return it->second;
        }
        // Should never happen if initialized correctly
        return 0;
    }
}

