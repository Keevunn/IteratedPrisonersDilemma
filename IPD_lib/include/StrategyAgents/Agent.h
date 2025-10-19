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

        virtual ~Agent() = default;

        virtual ResponseType decide(const ResponseType& lastResponse);

        virtual void resetAgent();

        [[nodiscard]] double getScore() const;
        void setScore(double value);
        virtual void addToScore(double value);

        [[nodiscard]] std::string_view getName() const;

        friend std::ostream& operator<<(std::ostream& os, const Agent& agent);

    protected:
        ResponseType noisyResponse(const ResponseType& response);
        const ResponseType initialResponse = ResponseType::C;
        std::string name{};

    private:
        double score{}; // Score per match

    };

}