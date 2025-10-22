#include <gtest/gtest.h>
#include "../include/Match.h"
#include "../include/StrategyAgents/ALLC.h"
#include "../include/StrategyAgents/ALLD.h"
#include "../include/StrategyAgents/TFT.h"
#include "../include/GameConfig.h"

using namespace Tournament::Match;
using namespace StrategyAgents;
using namespace GameConfig::Responses;

class Match_MechanicsTests : public ::testing::Test {
protected:
    void SetUp() override {
        GameConfig::GameConfig::rounds = 5;
        GameConfig::GameConfig::repeats = 1;
        GameConfig::GameConfig::seed = 0;
        GameConfig::GameConfig::epsilon = 0.0;
        GameConfig::GameConfig::payoffs = {5, 3, 1, 0};
    }
};

// ==================== Payoff Calculation Tests ====================

TEST_F(Match_MechanicsTests, CalculatePayoff_MutualCooperation) {
    EXPECT_EQ(calculatePayoff(ResponseType::C, ResponseType::C), 3.0); // R
}

TEST_F(Match_MechanicsTests, CalculatePayoff_MutualDefection) {
    EXPECT_EQ(calculatePayoff(ResponseType::D, ResponseType::D), 1.0); // P
}

TEST_F(Match_MechanicsTests, CalculatePayoff_Temptation) {
    EXPECT_EQ(calculatePayoff(ResponseType::D, ResponseType::C), 5.0); // T
}

TEST_F(Match_MechanicsTests, CalculatePayoff_Sucker) {
    EXPECT_EQ(calculatePayoff(ResponseType::C, ResponseType::D), 0.0); // S
}

// ==================== Match Outcome Tests ====================

TEST_F(Match_MechanicsTests, ALLC_vs_ALLC_BothCooperate) {
    const std::unique_ptr<Agent> allc1 = std::make_unique<ALLC>();
    const std::unique_ptr<Agent> allc2 = std::make_unique<ALLC>();
    
    simulateMatch(allc1, allc2);
    
    // 5 rounds × 3 (R) = 15 each
    EXPECT_EQ(allc1->getScore(), 15.0);
    EXPECT_EQ(allc2->getScore(), 15.0);
}

TEST_F(Match_MechanicsTests, ALLC_vs_ALLD_ExploitedCompletely) {
    const std::unique_ptr<Agent> allc = std::make_unique<ALLC>();
    const std::unique_ptr<Agent> alld = std::make_unique<ALLD>();
    
    simulateMatch(allc, alld);
    
    // ALLC: 5 rounds × 0 (S) = 0
    // ALLD: 5 rounds × 5 (T) = 25
    EXPECT_EQ(allc->getScore(), 0.0);
    EXPECT_EQ(alld->getScore(), 25.0);
}

TEST_F(Match_MechanicsTests, ALLD_vs_ALLD_MutualPunishment) {
    const std::unique_ptr<Agent> alld1 = std::make_unique<ALLD>();
    const std::unique_ptr<Agent> alld2 = std::make_unique<ALLD>();
    
    simulateMatch(alld1, alld2);
    
    // 5 rounds × 1 (P) = 5 each
    EXPECT_EQ(alld1->getScore(), 5.0);
    EXPECT_EQ(alld2->getScore(), 5.0);
}

TEST_F(Match_MechanicsTests, TFT_vs_TFT_MutualCooperation) {
    const std::unique_ptr<Agent> tft1 = std::make_unique<TFT>();
    const std::unique_ptr<Agent> tft2 = std::make_unique<TFT>();
    
    simulateMatch(tft1, tft2);
    
    // Both cooperate every round: 5 × 3 = 15
    EXPECT_EQ(tft1->getScore(), 15.0);
    EXPECT_EQ(tft2->getScore(), 15.0);
}

TEST_F(Match_MechanicsTests, TFT_vs_ALLD_OneExploitationThenPunishment) {
    const std::unique_ptr<Agent> tft = std::make_unique<TFT>();
    const std::unique_ptr<Agent> alld = std::make_unique<ALLD>();
    
    simulateMatch(tft, alld);
    
    // Round 1: TFT cooperates, ALLD defects → TFT gets 0, ALLD gets 5
    // Rounds 2-5: Both defect → each gets 1 per round (4 rounds = 4)
    // TFT total: 0 + 4 = 4
    // ALLD total: 5 + 4 = 9
    EXPECT_EQ(tft->getScore(), 4.0);
    EXPECT_EQ(alld->getScore(), 9.0);
}

TEST_F(Match_MechanicsTests, TFT_vs_ALLC_AlwaysCooperate) {
    const std::unique_ptr<Agent> tft = std::make_unique<TFT>();
    const std::unique_ptr<Agent> allc = std::make_unique<ALLC>();
    
    simulateMatch(tft, allc);
    
    // Both cooperate: 5 × 3 = 15
    EXPECT_EQ(tft->getScore(), 15.0);
    EXPECT_EQ(allc->getScore(), 15.0);
}

// ==================== Score Accumulation Tests ====================

TEST_F(Match_MechanicsTests, Scores_AccumulateCorrectly) {
    auto agent = std::make_unique<ALLC>();
    
    agent->addToScore(5.0);
    EXPECT_EQ(agent->getScore(), 5.0);
    
    agent->addToScore(3.0);
    EXPECT_EQ(agent->getScore(), 8.0);
    
    agent->addToScore(1.5);
    EXPECT_EQ(agent->getScore(), 9.5);
}

TEST_F(Match_MechanicsTests, Reset_ClearsScores) {
    auto agent = std::make_unique<ALLC>();
    
    agent->addToScore(100.0);
    EXPECT_EQ(agent->getScore(), 100.0);
    
    agent->resetAgent();
    EXPECT_EQ(agent->getScore(), 0.0);
}

// ==================== Round Execution Tests ====================

TEST_F(Match_MechanicsTests, RunRound_ReturnsCorrectResponses) {
    const std::unique_ptr<Agent> allc = std::make_unique<ALLC>();
    const std::unique_ptr<Agent> alld = std::make_unique<ALLD>();
    
    auto [p1Response, p2Response] = runRound(allc, alld);
    
    EXPECT_EQ(p1Response, ResponseType::C);
    EXPECT_EQ(p2Response, ResponseType::D);
}

TEST_F(Match_MechanicsTests, RunRound_UpdatesScores) {
    const std::unique_ptr<Agent> allc = std::make_unique<ALLC>();
    const std::unique_ptr<Agent> alld = std::make_unique<ALLD>();
    
    runRound(allc, alld);
    
    EXPECT_EQ(allc->getScore(), 0.0); // Sucker
    EXPECT_EQ(alld->getScore(), 5.0); // Temptation
}
