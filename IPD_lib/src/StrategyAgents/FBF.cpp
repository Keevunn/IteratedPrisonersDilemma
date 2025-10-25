#include "../../include/StrategyAgents/FBF.h"

namespace StrategyAgents {

    void FBF::addToScore(const double value) {
        lastPayoff = value;
        Agent::addToScore(value);
    }

    ResponseType FBF::decide(const ResponseType& lastResponse) {
        if (lastResponse == ResponseType::INVALID) return noisyResponse(response); // initial
        if (lastResponse == ResponseType::C) opponentCanCooperate = true;
        if (apologisePhase > 0) {
            apologisePhase--;
            response = (opponentCanCooperate) ? ResponseType::C : ResponseType::D; // opponent hasn't proved to be trustworthy yet
            // If defection was caused by noise still counts cooperation
            if (lastPayoff == GameConfig::GameConfig::payoffs[1]) mutualCooperationStreak++;
            return noisyResponse(response);
        }

        // If retaliating, continue punishment
        if (remainingRetaliation > 0) {
            remainingRetaliation--;

            if (remainingRetaliation == 0) apologisePhase = MAX_FREEZE;

            response = ResponseType::D;
            return noisyResponse(response);
        }

        // Check if exploited - if opponent is trusted (mutual cooperation streak) ignores one round of defection
        if (mutualCooperationStreak < FORGIVENESS_THRESHOLD && lastPayoff == GameConfig::GameConfig::payoffs[3]) {
            exploitationCount++;

            // Escalate punishment based on exploitation history, but cap it
            int punishmentRounds = std::min(BASE_RETALIATION + exploitationCount, MAX_RETALIATION);
            remainingRetaliation = punishmentRounds - 1; // retaliating current round so subtract 1

            response = ResponseType::D;
            return noisyResponse(response);
        }

        // Track opponent's cooperation for forgiveness
        if (lastPayoff == GameConfig::GameConfig::payoffs[1]) {
            mutualCooperationStreak++;
            // After sustained cooperation, reduce exploitation memory
            if (mutualCooperationStreak >= FORGIVENESS_THRESHOLD && exploitationCount > 0) {
                exploitationCount--;
                mutualCooperationStreak = 0;
            }
        } else mutualCooperationStreak = 0;

        // Default: cooperate
        response = (opponentCanCooperate) ? ResponseType::C : ResponseType::D;
        return noisyResponse(response);
    }

    void FBF::resetAgent() {
        Agent::resetAgent();
        response = initialResponse;
        lastPayoff = 0;
        exploitationCount = 0;
        mutualCooperationStreak = 0;
        opponentCanCooperate = false;
        apologisePhase = 0;
        remainingRetaliation = 0;
    }
}