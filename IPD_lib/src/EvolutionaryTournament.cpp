#include "../include/EvolutionaryTournament.h"

#include <numeric>
#include <ranges>

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
            resetFitness();
        }
    }

    std::unordered_map<StrategyTypes, std::vector<double>>& EvolutionaryTournament::getProportionHistory() { return proportionHistory; }

    std::unordered_map<StrategyTypes, std::vector<double>> & EvolutionaryTournament::getAvgFitnessHistory() { return avgFitnessHistory; }

    void EvolutionaryTournament::runTournamentMatches() {
        for (auto& match : matchResults) {
            std::vector<double> p1MeanPayoff{};
            std::vector<double> p2MeanPayoff{};
            p1MeanPayoff.reserve(GameConfig::GameConfig::repeats);
            p2MeanPayoff.reserve(GameConfig::GameConfig::repeats);

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
        for (const auto& strat : strategies) {
            strategyProportion[strat] = equalShare;
            strategyFitness[strat] = 0.0;
            proportionHistory[strat] = {};
            avgFitnessHistory[strat] = {};
        }
    }

    // Returns expected fitness for the entire population
    void EvolutionaryTournament::calculateExpectedFitness() {
        for (const auto& match : matchResults) {
            StrategyTypes p1Strat = match.player1->getStrategy();
            StrategyTypes p2Strat = match.player2->getStrategy();

            strategyFitness[p1Strat] += match.p1Mean * strategyProportion[p2Strat];
            strategyFitness[p2Strat] += match.p2Mean * strategyProportion[p1Strat];
        }
    }

    double EvolutionaryTournament::calculateAverageFitness() const {
        double avg = 0;
        for (const auto& strat : strategies) {
            avg += strategyFitness.at(strat) * strategyProportion.at(strat);
        }
        return avg;
    }

    void EvolutionaryTournament::updateProportions() {
        double avgFitness = calculateAverageFitness();
        for (auto& [strat, proportion] : strategyProportion) {
            proportion = proportion * (strategyFitness.at(strat) / avgFitness);
        }
    }

    void EvolutionaryTournament::resetFitness() {
        for (const auto &strat: strategyFitness | std::views::keys)
            strategyFitness.at(strat) = 0;
    }

    void EvolutionaryTournament::mutatePopulation() {
        for (auto& [strat, proportion] : strategyProportion) {
            if (Random::Random::getRandomDouble() >= mutation) continue;

            double reduction = proportion * (Random::Random::getRandomDouble() * 0.02); // Reduces the proportion by a random value between 1-2%
            strategyProportion.at(strat) -= reduction;

            // Redistribute the reduction to random other strategy
            StrategyTypes newStrat = strat;
            while (newStrat != strat)
                newStrat = strategies[static_cast<int>(Random::Random::getRandomDouble() * strategies.size())];
            strategyProportion.at(newStrat) += reduction;
        }
    }

    void EvolutionaryTournament::updateHistory() {
        for (const auto& [strat, proportion] : strategyProportion) {
            proportionHistory.at(strat).emplace_back(proportion);
            avgFitnessHistory.at(strat).emplace_back(strategyFitness.at(strat));
        }
    }
}

