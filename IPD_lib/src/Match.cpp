#include "../include/Match.h"
#include "../include/MathUtil.h"

#include <stdexcept>

namespace Tournament::Match {

    std::pair<double, double> simulateMatch(const std::unique_ptr<StrategyAgents::Agent>& player1, const std::unique_ptr<StrategyAgents::Agent>& player2) {

        std::pair<ResponseType, ResponseType> roundResult = runRound(player1, player2);
        for (int i{}; i < GameConfig::GameConfig::rounds - 1; ++i) {
            roundResult = runRound(player1, player2, roundResult.first, roundResult.second);
        }

        return std::make_pair(player1->getScore(), player2->getScore());
    }

    std::pair<ResponseType, ResponseType> runRound(
        const std::unique_ptr<StrategyAgents::Agent>& player1, const std::unique_ptr<StrategyAgents::Agent>& player2,
        const ResponseType& p1LastResponse, const ResponseType& p2LastResponse) {
        ResponseType p1Response = player1->decide(p2LastResponse);
        ResponseType p2Response = player2->decide(p1LastResponse);

        const double p1Score = calculatePayoff(p1Response, p2Response);
        player1->addToScore(p1Score);
        const double p2Score = calculatePayoff(p2Response, p1Response);
        player2->addToScore(p2Score);

        return std::make_pair(p1Response, p2Response);
    }

    double calculatePayoff(const ResponseType& p1, const ResponseType& p2) {
        const auto& p = GameConfig::GameConfig::payoffs;

        if (p1 == ResponseType::D && p2 == ResponseType::C) return p[0]; // Temptation
        if (p1 == ResponseType::C && p2 == ResponseType::C) return p[1]; // Reward
        if (p1 == ResponseType::D && p2 == ResponseType::D) return p[2]; // Punishment
        if (p1 == ResponseType::C && p2 == ResponseType::D) return p[3]; // Sucker

        throw std::invalid_argument("Invalid move pair");
    }

}
