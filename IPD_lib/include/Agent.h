#pragma once
#include <string>

// Define an abstract base class for each strategy to inherit from

namespace StrategyAgents {

    class Agent {
    public:
        virtual ~Agent() = default;

        virtual std::string decide(const std::string_view& lastResponse);
        virtual std::string decide(); // Can be used for memoryless strats or initial decision

        double getScore() const;
        void setScore(double value);
        void addToScore(double value);

    private:
        double score{}; // Score per round
        std::string initialResponse;
    };

}