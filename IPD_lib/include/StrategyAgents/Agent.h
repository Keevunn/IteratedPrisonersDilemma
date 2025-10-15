#pragma once
#include "../GameConfig.h"

#include <string>

using namespace GameConfig::Responses;

// Define an abstract base class for each strategy to inherit from

namespace StrategyAgents {

    class Agent {
    public:
        explicit Agent(const ResponseType init) : initialResponse(init) {}
        Agent() = default;
        Agent(Agent&& other) noexcept : initialResponse(other.initialResponse) {} // move constructor, could move score, but not expecting to move Agent object after running rounds

        virtual ~Agent() = default;

        ResponseType decide(const ResponseType& lastResponse = ResponseType::INVALID);

        [[nodiscard]] double getScore() const;
        void setScore(double value);
        virtual void addToScore(double value);

    protected:
        virtual ResponseType decideLogic(const ResponseType& lastResponse);
        const ResponseType initialResponse = ResponseType::C;

    private:
        double score{}; // Score per round
    };

}