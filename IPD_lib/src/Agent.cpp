#include "../include/Agent.h"

namespace StrategyAgents {

    double Agent::getScore() const{ return score; }
    void Agent::setScore(double value) { score = value; }
    void Agent::addToScore(double value) { score += value; }

}