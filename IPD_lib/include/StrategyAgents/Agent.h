#pragma once

#include "../GameConfig.h"

using namespace GameConfig::Responses;
using namespace GameConfig::Strategies;

// Define an abstract base class for each strategy to inherit from

namespace StrategyAgents {

    class Agent {
    public:
        explicit Agent(const ResponseType init) : initialResponse(init) {} // TODO Can remove?
        explicit Agent(const ResponseType init, const StrategyTypes strat) : initialResponse(init), strategy(strat) {
            name = strategyToString(strat);
        }
        explicit Agent(const StrategyTypes strat) : strategy(strat) {
            name = strategyToString(strat);
        }
        Agent() = default;

        virtual ~Agent() = default;

        virtual ResponseType decide(const ResponseType& lastResponse);

        virtual void resetAgent();

        [[nodiscard]] double getScore() const;
        void setScore(double value);
        virtual void addToScore(double value);

        [[nodiscard]] std::string_view getName() const;
        [[nodiscard]] StrategyTypes getStrategy() const;

        friend std::ostream& operator<<(std::ostream& os, const Agent& agent);

    protected:
        ResponseType noisyResponse(const ResponseType& response);
        const ResponseType initialResponse = ResponseType::C;
        std::string name{};
        StrategyTypes strategy;

    private:
        double score{}; // Score per match

    };

}