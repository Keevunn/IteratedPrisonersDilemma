#include "../include/Tournament.h"
#include "../include/Match.h"

#include "../include/StrategyAgents/ALLC.h"
#include "../include/StrategyAgents/ALLD.h"
#include "../include/StrategyAgents/CONTRITE.h"
#include "../include/StrategyAgents/GRIM.h"
#include "../include/StrategyAgents/PAVLOV.h"
#include "../include/StrategyAgents/PROBER.h"
#include "../include/StrategyAgents/RNDp.h"
#include "../include/StrategyAgents/TFT.h"

#include "../include/MathUtil.h"

namespace Tournament {

    std::vector<MatchResults>& Tournament::simulateTournament() {

        for (auto& match : matchResults) { // Iterate through each match up
            std::vector<double> player1Scores{};
            std::vector<double> player2Scores{};
            player1Scores.reserve(GameConfig::GameConfig::repeats);
            player2Scores.reserve(GameConfig::GameConfig::repeats);

            for (int i{};  i < GameConfig::GameConfig::repeats; i++) { // Play each match "repeats" times
                Match::simulateMatch(match.player1, match.player2);
                player1Scores.emplace_back(match.player1->getScore());
                player2Scores.emplace_back(match.player2->getScore());
                // Reset score for each player so next match starts fresh
                match.player1->resetAgent();
                match.player2->resetAgent();
            }
            // Calculate mean, stDev, CI for each match
            match.p1Mean = MathUtil::calculateMean(player1Scores, GameConfig::GameConfig::repeats);
            match.p1StdDev = MathUtil::calculateStdDev(player1Scores, GameConfig::GameConfig::repeats, match.p1Mean);
            match.p1CI = MathUtil::calculateCI( match.p1Mean, match.p1StdDev, GameConfig::GameConfig::repeats );

            match.p2Mean = MathUtil::calculateMean(player2Scores, GameConfig::GameConfig::repeats);
            match.p2StdDev = MathUtil::calculateStdDev(player2Scores, GameConfig::GameConfig::repeats, match.p2Mean);
            match.p2CI = MathUtil::calculateCI( match.p2Mean, match.p2StdDev, GameConfig::GameConfig::repeats );

            std::ranges::copy(player1Scores, back_inserter(totalScores[match.player1->getName().data()]) );
            std::ranges::copy(player2Scores, back_inserter(totalScores[match.player2->getName().data()]) );

        }

        return matchResults;

    }

    std::unordered_map<std::string, std::vector<double>> Tournament::getTotalScores() { return totalScores; }

    std::vector<MatchResults> Tournament::getMatchResults() { return matchResults; }

    void Tournament::matchMaking() {
        generatePlayers();

        for (const auto& p1Strat : strategies)
            for (const auto& p2Strat : strategies)
                matchResults.emplace_back(players[p1Strat].first, players[p2Strat].second);

    }

    // Stored as 2 unique instances to allow self-play
    void Tournament::generatePlayers() {
        if (!players.empty()) return;

        for (const auto& strat : strategies) {
            players.emplace(
                strat,
                std::make_pair(getAgentType(strat), getAgentType(strat))
                );
            // creates an empty vector for every strategy to store scores across every game
            totalScores.emplace(players[strat].first->getName(), std::vector<double>{});
        }
    }


    std::unique_ptr<StrategyAgents::Agent> Tournament::getAgentType(const StrategyTypes& strat) {
        switch (strat) {
            case (StrategyTypes::ALLC):
                return std::make_unique<StrategyAgents::ALLC>(); break;
            case (StrategyTypes::ALLD):
                return std::make_unique<StrategyAgents::ALLD>(); break;
            case (StrategyTypes::CONTRITE):
                return std::make_unique<StrategyAgents::CONTRITE>(); break;
            case (StrategyTypes::GRIM):
                return std::make_unique<StrategyAgents::GRIM>(); break;
            case (StrategyTypes::PAVLOV):
                return std::make_unique<StrategyAgents::PAVLOV>(); break;
            case (StrategyTypes::PROBER):
                return std::make_unique<StrategyAgents::PROBER>(); break;
            case (StrategyTypes::RND03):
                return std::make_unique<StrategyAgents::RNDp>(); break;
            case (StrategyTypes::TFT):
                return std::make_unique<StrategyAgents::TFT>(); break;
            default:
                throw std::invalid_argument("Invalid Strategy Type"); // Should never be able to reach this line
        }
    }

    // Outputs 2 rows for CSV file
    // Strategy, Opponent, mean, stdDev, CI_LB, CI_UB, rounds, repeats, epsilon, T, R, P, S
    std::ostream& operator<<(std::ostream& os, const MatchResultsStruct& results){
        os << results.player1->getName() << ", "
        << results.player2->getName() << ", "
        << results.p1Mean << ", "
        << results.p1StdDev << ", "
        << results.p1CI.first << ", "
        << results.p1CI.second << ", "
        << GameConfig::GameConfig::rounds << ", "
        << GameConfig::GameConfig::repeats << ", "
        << GameConfig::GameConfig::seed << ", "
        << GameConfig::GameConfig::epsilon << ", "
        << GameConfig::GameConfig::payoffs[0] << ", "
        << GameConfig::GameConfig::payoffs[1] << ", "
        << GameConfig::GameConfig::payoffs[2] << ", "
        << GameConfig::GameConfig::payoffs[3]
        << std::endl;

        os << results.player2->getName() << ", "
        << results.player1->getName() << ", "
        << results.p2Mean << ", "
        << results.p2StdDev << ", "
        << results.p2CI.first << ", "
        << results.p2CI.second << ", "
        << GameConfig::GameConfig::rounds << ", "
        << GameConfig::GameConfig::repeats << ", "
        << GameConfig::GameConfig::seed << ", "
        << GameConfig::GameConfig::epsilon << ", "
        << GameConfig::GameConfig::payoffs[0] << ", "
        << GameConfig::GameConfig::payoffs[1] << ", "
        << GameConfig::GameConfig::payoffs[2] << ", "
        << GameConfig::GameConfig::payoffs[3]
        << std::endl;

        return os;
    }


}
