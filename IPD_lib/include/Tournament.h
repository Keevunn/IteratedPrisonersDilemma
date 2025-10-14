#pragma once
#include "GameConfig.h"

#include <vector>

#include "Agent.h"

using namespace GameConfig::Strategies;

namespace Tournament {

    typedef struct MatchResults {
        MatchResults() = default;

        StrategyAgents::Agent player1;
        StrategyAgents::Agent player2;

        std::vector<double> player1Scores{};
        std::vector<double> player2Scores{};
    };

    class Tournament {
    public:
        Tournament() = default;

        void simulateTournament();

    private:
        void matchMaking();

        std::vector<StrategyTypes> strategies;
        std::vector<MatchResults> matchResults{}; // Each item is a struct, representing the results of a repeated match between 2 strategies

    };

}

