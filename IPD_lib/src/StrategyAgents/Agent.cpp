#include "../../include/StrategyAgents/Agent.h"

namespace StrategyAgents {

    double Agent::getScore() const{ return score; }
    void Agent::setScore(const double value) { score = value; }
    void Agent::addToScore(const double value) { score += value; }

    ResponseType Agent::decide(const ResponseType& lastResponse) { return decideLogic(lastResponse); }

}