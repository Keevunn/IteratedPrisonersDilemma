#include <gtest/gtest.h>
#include "../include/StrategyAgents/ALLC.h"
#include "../include/StrategyAgents/ALLD.h"
#include "../include/StrategyAgents/TFT.h"
#include "../include/StrategyAgents/GRIM.h"
#include "../include/StrategyAgents/PAVLOV.h"
#include "../include/StrategyAgents/CONTRITE.h"
#include "../include/StrategyAgents/PROBER.h"
#include "../include/StrategyAgents/CAUTIOUS.h"
#include "../include/StrategyAgents/FBF.h"
#include "../include/GameConfig.h"

using namespace StrategyAgents;
using namespace GameConfig::Responses;

// Helper to reset game config to defaults
class StrategyTest : public ::testing::Test {
protected:
    void SetUp() override {
        GameConfig::GameConfig::rounds = 100;
        GameConfig::GameConfig::repeats = 1;
        GameConfig::GameConfig::seed = 0;
        GameConfig::GameConfig::epsilon = 0.0;
        GameConfig::GameConfig::payoffs = {5, 3, 1, 0};
    }
};

// ==================== ALLC Tests ====================

TEST_F(StrategyTest, ALLC_AlwaysCooperates) {
    ALLC agent;
    EXPECT_EQ(agent.decide(ResponseType::INVALID), ResponseType::C);
    EXPECT_EQ(agent.decide(ResponseType::C), ResponseType::C);
    EXPECT_EQ(agent.decide(ResponseType::D), ResponseType::C);
}

TEST_F(StrategyTest, ALLC_HasCorrectComplexity) {
    ALLC agent;
    EXPECT_EQ(agent.getComplexityCost(), 1);
}

// ==================== ALLD Tests ====================

TEST_F(StrategyTest, ALLD_AlwaysDefects) {
    ALLD agent;
    EXPECT_EQ(agent.decide(ResponseType::INVALID), ResponseType::D);
    EXPECT_EQ(agent.decide(ResponseType::C), ResponseType::D);
    EXPECT_EQ(agent.decide(ResponseType::D), ResponseType::D);
}

TEST_F(StrategyTest, ALLD_HasCorrectComplexity) {
    ALLD agent;
    EXPECT_EQ(agent.getComplexityCost(), 1);
}

// ==================== TFT Tests ====================

TEST_F(StrategyTest, TFT_StartsWithCooperation) {
    TFT agent;
    EXPECT_EQ(agent.decide(ResponseType::INVALID), ResponseType::C);
}

TEST_F(StrategyTest, TFT_MirrorsOpponent) {
    TFT agent;
    agent.decide(ResponseType::INVALID); // First move
    EXPECT_EQ(agent.decide(ResponseType::C), ResponseType::C);
    EXPECT_EQ(agent.decide(ResponseType::D), ResponseType::D);
    EXPECT_EQ(agent.decide(ResponseType::C), ResponseType::C);
}

TEST_F(StrategyTest, TFT_HasCorrectComplexity) {
    TFT agent;
    EXPECT_EQ(agent.getComplexityCost(), 2);
}

// ==================== GRIM Tests ====================

TEST_F(StrategyTest, GRIM_StartsWithCooperation) {
    GRIM agent;
    EXPECT_EQ(agent.decide(ResponseType::INVALID), ResponseType::C);
}

TEST_F(StrategyTest, GRIM_CooperatesUntilDefection) {
    GRIM agent;
    agent.decide(ResponseType::INVALID);
    EXPECT_EQ(agent.decide(ResponseType::C), ResponseType::C);
    EXPECT_EQ(agent.decide(ResponseType::C), ResponseType::C);
}

TEST_F(StrategyTest, GRIM_DefectsForeverAfterBetrayal) {
    GRIM agent;
    agent.decide(ResponseType::INVALID);
    agent.decide(ResponseType::C);
    EXPECT_EQ(agent.decide(ResponseType::D), ResponseType::D); // Betrayed, now defects
    EXPECT_EQ(agent.decide(ResponseType::C), ResponseType::D); // Forever
    EXPECT_EQ(agent.decide(ResponseType::C), ResponseType::D);
}

TEST_F(StrategyTest, GRIM_HasCorrectComplexity) {
    GRIM agent;
    EXPECT_EQ(agent.getComplexityCost(), 2);
}

// ==================== PAVLOV Tests ====================

TEST_F(StrategyTest, PAVLOV_StartsWithCooperation) {
    PAVLOV agent;
    EXPECT_EQ(agent.decide(ResponseType::INVALID), ResponseType::C);
}

TEST_F(StrategyTest, PAVLOV_WinStayLoseShift) {
    PAVLOV agent;
    agent.decide(ResponseType::INVALID);
    
    // Win (R) - stay
    agent.addToScore(GameConfig::GameConfig::payoffs[1]); // Reward
    EXPECT_EQ(agent.decide(ResponseType::C), ResponseType::C);

    // Lose (S) - shift
    agent.addToScore(GameConfig::GameConfig::payoffs[3]); // Sucker
    EXPECT_EQ(agent.decide(ResponseType::D), ResponseType::D);
    
    // Win (P) - stay
    agent.addToScore(GameConfig::GameConfig::payoffs[2]); // Punishment
    EXPECT_EQ(agent.decide(ResponseType::D), ResponseType::D);
    

}

TEST_F(StrategyTest, PAVLOV_HasCorrectComplexity) {
    PAVLOV agent;
    EXPECT_EQ(agent.getComplexityCost(), 2);
}

// ==================== CONTRITE Tests ====================

TEST_F(StrategyTest, CONTRITE_StartsWithCooperation) {
    CONTRITE agent;
    EXPECT_EQ(agent.decide(ResponseType::INVALID), ResponseType::C);
}

TEST_F(StrategyTest, CONTRITE_HasCorrectComplexity) {
    CONTRITE agent;
    EXPECT_EQ(agent.getComplexityCost(), 3);
}

// ==================== PROBER Tests ====================

TEST_F(StrategyTest, PROBER_ProbeSequence) {
    PROBER agent;
    // Probe phase: C, D, C, C
    EXPECT_EQ(agent.decide(ResponseType::INVALID), ResponseType::C);
    agent.addToScore(3.0);
    EXPECT_EQ(agent.decide(ResponseType::C), ResponseType::D);
    agent.addToScore(5.0);
    EXPECT_EQ(agent.decide(ResponseType::C), ResponseType::C);
    agent.addToScore(3.0);
    EXPECT_EQ(agent.decide(ResponseType::C), ResponseType::C);
}

TEST_F(StrategyTest, PROBER_HasCorrectComplexity) {
    PROBER agent;
    EXPECT_EQ(agent.getComplexityCost(), 3);
}

// ==================== Reset Tests ====================

TEST_F(StrategyTest, AllStrategies_ResetCorrectly) {
    ALLC allc;
    ALLD alld;
    TFT tft;
    GRIM grim;
    PAVLOV pavlov;
    
    // Add scores
    allc.addToScore(100.0);
    alld.addToScore(100.0);
    tft.addToScore(100.0);
    grim.addToScore(100.0);
    pavlov.addToScore(100.0);
    
    // Reset
    allc.resetAgent();
    alld.resetAgent();
    tft.resetAgent();
    grim.resetAgent();
    pavlov.resetAgent();
    
    // Check scores are zero
    EXPECT_EQ(allc.getScore(), 0.0);
    EXPECT_EQ(alld.getScore(), 0.0);
    EXPECT_EQ(tft.getScore(), 0.0);
    EXPECT_EQ(grim.getScore(), 0.0);
    EXPECT_EQ(pavlov.getScore(), 0.0);
}

// ==================== Custom Strategies Tests ====================

TEST_F(StrategyTest, CAUTIOUS_StartsWithDefection) {
    CAUTIOUS agent;
    EXPECT_EQ(agent.decide(ResponseType::INVALID), ResponseType::D);
}

TEST_F(StrategyTest, CAUTIOUS_HasCorrectComplexity) {
    CAUTIOUS agent;
    EXPECT_EQ(agent.getComplexityCost(), 2);
}

TEST_F(StrategyTest, FBF_StartsWithCooperation) {
    FBF agent;
    EXPECT_EQ(agent.decide(ResponseType::INVALID), ResponseType::C);
}

TEST_F(StrategyTest, FBF_HasCorrectComplexity) {
    FBF agent;
    EXPECT_EQ(agent.getComplexityCost(), 3);
}
