#pragma once
#include "StrategyAgents/Agent.h"
#include "GameConfig.h"

#include <vector>



namespace Tournament {

    // Manages a single match
    namespace Match {

        std::pair<double, double> simulateMatch(const std::unique_ptr<StrategyAgents::Agent>& player1, const std::unique_ptr<StrategyAgents::Agent>& player2);
        [[nodiscard]] std::pair<ResponseType, ResponseType> runRound(
            const std::unique_ptr<StrategyAgents::Agent>& player1, const std::unique_ptr<StrategyAgents::Agent>& player2,
            const ResponseType& p1LastResponse = ResponseType::INVALID, const ResponseType& p2LastResponse = ResponseType::INVALID); // Returns {player 1, player 2} outcome
        [[nodiscard]] double calculatePayoff(const ResponseType& p1, const ResponseType& p2); // ensures return value is used

    }

}
