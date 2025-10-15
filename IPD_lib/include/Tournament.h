#pragma once
#include "GameConfig.h"

#include <vector>

#include "StrategyAgents/Agent.h"

using namespace GameConfig::Strategies;

namespace Tournament {
// Note: Output is up to my discretion!!!
    typedef struct MatchResults {
        MatchResults(const StrategyAgents::Agent& p1, const StrategyAgents::Agent& p2) : player1(p1), player2(p2) {};

        const StrategyAgents::Agent& player1;
        const StrategyAgents::Agent& player2;

        std::vector<double> player1Scores{};
        std::vector<double> player2Scores{};

        friend std::ostream& operator<<(std::ostream& os, const MatchResults& results);
    };

    class Tournament {
    public:
        Tournament() = default;
        Tournament(const std::vector<StrategyTypes>& strats) : strategies(strats) {
            const size_t numMatches = strategies.size() * (strategies.size() - 1);
            matchResults.reserve(numMatches);
        }

        void simulateTournament();

    private:
        void matchMaking();

        const std::vector<StrategyTypes>& strategies;
        std::vector<MatchResults> matchResults{}; // Each item is a struct, representing the results of a repeated match between 2 strategies

    };

}

