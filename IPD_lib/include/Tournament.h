#pragma once
#include <unordered_map>

#include "StrategyAgents/Agent.h"

#include <vector>

using namespace GameConfig::Strategies;
using namespace GameConfig::Responses;

namespace Tournament {
// Note: Output is up to my discretion!!!

    typedef struct MatchResultsStruct {
        MatchResultsStruct( std::unique_ptr<StrategyAgents::Agent>& p1,  std::unique_ptr<StrategyAgents::Agent>& p2) : player1(p1), player2(p2) {};

        std::unique_ptr<StrategyAgents::Agent>& player1;
        std::unique_ptr<StrategyAgents::Agent>& player2;

        double p1Mean{}; double p1StdDev{}; std::pair<double,double> p1CI {};
        double p2Mean{}; double p2StdDev{}; std::pair<double,double> p2CI {};

        friend std::ostream& operator<<(std::ostream& os, const MatchResultsStruct& results);
    } MatchResults;

    class Tournament {
    public:
        explicit Tournament(const std::vector<StrategyTypes>& strats) : strategies(strats) {
            const size_t numMatches = strategies.size() * strategies.size();
            matchResults.reserve(numMatches);
            matchMaking();
        }

        std::vector<MatchResults>& simulateTournament();
        std::unordered_map<std::string, std::vector<double>> getTotalScores();
        std::vector<MatchResults> getMatchResults();

    private:
        void matchMaking();
        void generatePlayers();
        [[nodiscard]] std::unique_ptr<StrategyAgents::Agent> getAgentType(const StrategyTypes& strat);

        const std::vector<StrategyTypes>& strategies;
        std::unordered_map<StrategyTypes, std::pair<std::unique_ptr<StrategyAgents::Agent>, std::unique_ptr<StrategyAgents::Agent>>> players;
        std::unordered_map<std::string, std::vector<double>> totalScores;
        std::vector<MatchResults> matchResults{}; // Each item is a struct, representing the results of a repeated match between 2 strategies

    };

}

