#include "../../include/StrategyAgents/FBF.h"

namespace StrategyAgents {

    void FBF::addToScore(const double value) {
        lastPayoff = value;
        Agent::addToScore(value);
    }

    ResponseType FBF::decide(const ResponseType& lastResponse) {
        if (lastResponse == ResponseType::INVALID) return noisyResponse(response); // initial

        // Count number of cooperation decisions if not punishing
        if (response == ResponseType::C && remainingRetaliation == 0) coopCount++;

        // Track opponent's cooperation for forgiveness
        if (lastPayoff == GameConfig::GameConfig::payoffs[1]) {
            mutualCooperationStreak++;
            // After sustained cooperation, reduce exploitation memory
            if (mutualCooperationStreak >= FORGIVENESS_THRESHOLD && exploitationCount > 0) {
                exploitationCount--;
                mutualCooperationStreak = 0;
            }
        } else mutualCooperationStreak = 0;

        // If retaliating, continue punishment
        if (remainingRetaliation > 0) {
            remainingRetaliation--;

            if (remainingRetaliation == 0) freezeResponse = MAX_FREEZE;

            response = ResponseType::D;
            return noisyResponse(response);
        }

        // Check if exploited
        if (coopCount >= 2 && lastPayoff == GameConfig::GameConfig::payoffs[3]) {
            exploitationCount++;

            // Escalate punishment based on exploitation history, but cap it
            int punishmentRounds = std::min(BASE_RETALIATION + exploitationCount - 1, MAX_RETALIATION);
            remainingRetaliation = punishmentRounds - 1; // retaliating current round so subtract 1

            response = ResponseType::D;
            coopCount = 0;
            return noisyResponse(response);
        }

        // Default: cooperate
        response = ResponseType::C;
        return noisyResponse(response);
    }

    void FBF::resetAgent() {
        Agent::resetAgent();
        response = initialResponse;
        lastPayoff = 0;
        exploitationCount = 0;
        mutualCooperationStreak = 0;
        coopCount = 1;
        freezeResponse = 0;
    }
}