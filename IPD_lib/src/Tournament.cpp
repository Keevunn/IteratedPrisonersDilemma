#include "../include/Tournament.h"

#include <sstream>

#include "../include/Match.h"
#include "../include/MathUtil.h"

#include "../include/StrategyAgents/ALLC.h"
#include "../include/StrategyAgents/ALLD.h"
#include "../include/StrategyAgents/CONTRITE.h"
#include "../include/StrategyAgents/GRIM.h"
#include "../include/StrategyAgents/PAVLOV.h"
#include "../include/StrategyAgents/PROBER.h"
#include "../include/StrategyAgents/RNDp.h"
#include "../include/StrategyAgents/TFT.h"
#include "../include/StrategyAgents/CAUTIOUS.h"
#include "../include/StrategyAgents/FBF.h"

namespace Tournament {

     // Outputs 2 rows for CSV file
     // Strategy, Opponent, mean, CI_LB, CI_UB, rounds, repeats, seed, epsilon, payoffs
     std::ostream& operator<<(std::ostream& os, const MatchResultsStruct& results){
         std::ostringstream metadata;
         {
             const std::string payoffs = "\"" + std::to_string(GameConfig::GameConfig::payoffs[0]) + "," +
                                         std::to_string(GameConfig::GameConfig::payoffs[1]) + "," +
                                         std::to_string(GameConfig::GameConfig::payoffs[2]) + "," +
                                         std::to_string(GameConfig::GameConfig::payoffs[3]) + "\"";
             metadata << GameConfig::GameConfig::rounds << "," << GameConfig::GameConfig::repeats << ","
                    << GameConfig::GameConfig::seed << "," << GameConfig::GameConfig::epsilon << "," << payoffs;
         }
         os << results.player1->getName() << ","
             << results.player2->getName() << ","
             << results.p1Mean << ","
             << results.p1CI.first << ","
             << results.p1CI.second << ","
             << metadata.str() << std::endl;

         os << results.player2->getName() << ","
             << results.player1->getName() << ","
             << results.p2Mean << ","
             << results.p2CI.first << ","
             << results.p2CI.second << ","
             << metadata.str() << std::endl;

         return os;
     }

    void Tournament::simulateTournament() {

        for (auto& match : matchResults) { // Iterate through each match up
            std::vector<double> player1Payoffs{};
            std::vector<double> player2Payoffs{};
            player1Payoffs.reserve(GameConfig::GameConfig::repeats);
            player2Payoffs.reserve(GameConfig::GameConfig::repeats);

            for (int i{};  i < GameConfig::GameConfig::repeats; i++) { // Play each match "repeats" times
                Match::simulateMatch(match.player1, match.player2);
                // For stat calculations of the whole tournament - stores average payoffs
                totalAvgPayoffs[match.player1->getStrategy()].emplace_back(match.player1->getScore()/GameConfig::GameConfig::rounds);
                totalAvgPayoffs[match.player2->getStrategy()].emplace_back(match.player2->getScore()/GameConfig::GameConfig::rounds);
                // For Stat calculations for the match
                player1Payoffs.emplace_back(match.player1->getScore()/GameConfig::GameConfig::rounds);
                player2Payoffs.emplace_back(match.player2->getScore()/GameConfig::GameConfig::rounds);
                // Reset score for each player so next match starts fresh
                match.player1->resetAgent();
                match.player2->resetAgent();
            }
            // Calculate mean, stDev, CI for each match
            match.p1Mean = MathUtil::calculateMean(player1Payoffs, GameConfig::GameConfig::repeats);
            const auto p1StdDev = MathUtil::calculateStdDev(player1Payoffs, GameConfig::GameConfig::repeats, match.p1Mean);
            match.p1CI = MathUtil::calculateCI( match.p1Mean, p1StdDev, GameConfig::GameConfig::repeats );

            match.p2Mean = MathUtil::calculateMean(player2Payoffs, GameConfig::GameConfig::repeats);
            const auto p2StdDev = MathUtil::calculateStdDev(player2Payoffs, GameConfig::GameConfig::repeats, match.p2Mean);
            match.p2CI = MathUtil::calculateCI( match.p2Mean, p2StdDev, GameConfig::GameConfig::repeats );
        }

    }

    std::unordered_map<StrategyTypes, std::vector<double>>& Tournament::getTotalPayoffs() { return totalAvgPayoffs; }

    std::vector<MatchResults>& Tournament::getMatchResults() { return matchResults; }

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
            totalAvgPayoffs.emplace(players[strat].first->getStrategy(), std::vector<double>{});
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
        case (StrategyTypes::CAUTIOUS):
            return std::make_unique<StrategyAgents::CAUTIOUS>(); break;
        case (StrategyTypes::FBF):
            return std::make_unique<StrategyAgents::FBF>(); break;
        default:
            throw std::invalid_argument("Invalid Strategy Type"); // Should never be able to reach this line
        }
    }


}
