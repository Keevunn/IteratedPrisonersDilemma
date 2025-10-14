#include "../include/Match.h"
#include "../include/GameConfig.h"

#include <stdexcept>

namespace Tournament {

    std::pair<double, double> Match::simulateMatch() {

        for (int i{}; i < GameConfig::rounds; ++i) {
            std::pair<std::string, std::string> roundResult = runRound();
            matchHistory.emplace_back(roundResult);
        }

        return std::make_pair(player1.getScore(), player2.getScore());
    }

    std::pair<std::string, std::string> Match::runRound() {
        std::string p1Response = player1.decide();
        std::string p2Response = player2.decide();

        double p1Score = calculatePayoff(p1Response, p2Response);
        player1.addToScore(p1Score);
        double p2Score = calculatePayoff(p2Response, p1Response);
        player2.addToScore(p2Score);

        return std::make_pair(p1Response, p2Response);
    }

    double Match::calculatePayoff(const std::string_view& p1, const std::string_view& p2) const {
        const auto& p = GameConfig::payoffs;

        if (p1 == "D" && p2 == "C") return p[0]; // Temptation
        if (p1 == "C" && p2 == "C") return p[1]; // Reward
        if (p1 == "D" && p2 == "D") return p[2]; // Punishment
        if (p1 == "C" && p2 == "D") return p[3]; // Sucker

        throw std::invalid_argument("Invalid move pair");
    }

}
