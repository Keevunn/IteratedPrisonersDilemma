#pragma once
#include "Agent.h"
#include "GameConfig.h"

#include <vector>



namespace Tournament {

    // Manages a single match
    class Match {

    public:
        Match(StrategyAgents::Agent p1, StrategyAgents::Agent p2) : player1(p1), player2(p2){
            matchHistory.reserve(GameConfig::rounds);
        }

    std::pair<double, double> simulateMatch();

    private:
        std::pair<std::string, std::string> runRound(); // Returns {player 1, player 2} outcome
        [[nodiscard]] double calculatePayoff(const std::string_view& p1, const std::string_view& p2) const; // ensures return value is used


        std::vector<std::pair<std::string, std::string>> matchHistory = {};
        StrategyAgents::Agent player1;
        StrategyAgents::Agent player2;

    };

}
