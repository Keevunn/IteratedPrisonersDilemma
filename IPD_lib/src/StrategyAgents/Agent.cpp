#include "../../include/StrategyAgents/Agent.h"

#include "../../include/RandomUtil.h"

namespace StrategyAgents {
    ResponseType Agent::decide(const ResponseType &lastResponse) { return ResponseType::INVALID; }

    void Agent::resetAgent() {
        score = 0;
    }

    double Agent::getScore() const{ return score; }
    void Agent::setScore(const double value) { score = value; }
    void Agent::addToScore(const double value) { score += value; }

    std::string_view Agent::getName() const { return name; }

    StrategyTypes Agent::getStrategy() const { return strategy; }

    ResponseType Agent::noisyResponse(const ResponseType &response) {
        bool shouldSwitch = false;

        if ( Random::Random::getRandomDouble() <= GameConfig::GameConfig::epsilon ) shouldSwitch = true;

        return shouldSwitch ?
            (( response == ResponseType::C ) ?
                ResponseType::D : ResponseType::C) :
            response;
    }
}
