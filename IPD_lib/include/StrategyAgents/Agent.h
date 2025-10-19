#pragma once
#include "../GameConfig.h"

using namespace GameConfig::Responses;

// Define an abstract base class for each strategy to inherit from

namespace StrategyAgents {

    class Agent {
    public:
        explicit Agent(const ResponseType init) : initialResponse(init) {}
        explicit Agent(const ResponseType init, const std::string_view name) : initialResponse(init), name(name) {}
        explicit Agent(const std::string_view name) : name(name) {}
        Agent() = default;
        Agent(Agent&& other) noexcept : initialResponse(other.initialResponse) {} // move constructor, could move score, but not expecting to move Agent object after running rounds

        virtual ~Agent() = default;

        virtual ResponseType decide(const ResponseType& lastResponse);

        virtual void resetAgent();

        [[nodiscard]] double getScore() const;
        void setScore(double value);
        virtual void addToScore(double value);

        std::string_view getName() const;

        friend std::ostream& operator<<(std::ostream& os, const Agent& agent);

    protected:
        const ResponseType initialResponse = ResponseType::C;
        std::string name{};

    private:
        double score{}; // Score per match

    };

}