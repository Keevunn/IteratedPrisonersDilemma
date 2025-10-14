#pragma once
#include <string>
#include <utility>

// Define an abstract base class for each strategy to inherit from

namespace StrategyAgents {

    class Agent {
    public:
        explicit Agent(std::string  init) : initialResponse(std::move(init)) {}
        Agent(Agent&& other) noexcept : initialResponse(other.initialResponse) {} // move constructor, could move score, but not expecting to move Agent object after running rounds

        virtual ~Agent() = default;

        virtual std::string decide(const std::string_view& lastResponse);
        virtual std::string decide(); // Can be used for memoryless strats or initial decision

        [[nodiscard]] double getScore() const;
        void setScore(double value);
        void addToScore(double value);

    private:
        double score{}; // Score per round
        const std::string_view initialResponse;
    };

}