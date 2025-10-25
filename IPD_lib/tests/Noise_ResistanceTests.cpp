#include <gtest/gtest.h>
#include "../include/MathUtil.h"
#include "../include/Tournament.h"
#include "../include/GameConfig.h"


using namespace GameConfig::Strategies;

class NoiseTest : public ::testing::Test {
protected:
    void SetUp() override {
        GameConfig::GameConfig::rounds = 100;
        GameConfig::GameConfig::repeats = 50; // More repeats for noise tests
        GameConfig::GameConfig::seed = 42;
        GameConfig::GameConfig::payoffs = {5, 3, 1, 0};
    }
};

// ==================== Noise Application Tests ====================

TEST_F(NoiseTest, NoNoise_DeterministicOutcomes) {
    GameConfig::GameConfig::epsilon = 0.0;
    
    std::vector<StrategyTypes> strategies = {
        StrategyTypes::ALLC,
        StrategyTypes::ALLD
    };
    
    Tournament::Tournament tournament(strategies);
    tournament.simulateTournament();
    
    auto& scores = tournament.getTotalPayoffs();
    
    // With no noise, all ALLC vs ALLD scores should be identical (0)
    // ALLC vs ALLD scores will be in indexes 50-149
    auto& allcScores = scores[StrategyTypes::ALLC];
    bool isIdentical = true;
    for (size_t i = 150; i < 198; ++i) {
        if (allcScores[i] != allcScores[i+1]) {
            isIdentical = false;
            break;
        }
    }
    EXPECT_TRUE(isIdentical);
}

TEST_F(NoiseTest, WithNoise_IncreasedVariance) {
    GameConfig::GameConfig::rounds = 100;
    GameConfig::GameConfig::repeats = 30;

    std::vector<StrategyTypes> strategies = {
        StrategyTypes::ALLC,
        StrategyTypes::ALLD
    };

    // Low noise
    GameConfig::GameConfig::epsilon = 0.01;
    Tournament::Tournament tournament1(strategies);
    tournament1.simulateTournament();

    // Higher noise
    GameConfig::GameConfig::epsilon = 0.10;
    Tournament::Tournament tournament2(strategies);
    tournament2.simulateTournament();

    auto& matches1 = tournament1.getMatchResults();
    auto& matches2 = tournament2.getMatchResults();

    // Find ALLC vs ALLD match in both tournaments
    double ci_width1 = 0.0, ci_width2 = 0.0;

    for (const auto& match : matches1) {
        if (match.player1->getStrategy() == StrategyTypes::ALLC &&
            match.player2->getStrategy() == StrategyTypes::ALLD) {
            ci_width1 = match.p1CI.second - match.p1CI.first;
            break;
            }
    }

    for (const auto& match : matches2) {
        if (match.player1->getStrategy() == StrategyTypes::ALLC &&
            match.player2->getStrategy() == StrategyTypes::ALLD) {
            ci_width2 = match.p2CI.second - match.p2CI.first;
            break;
            }
    }

    // Higher noise should result in wider CI for ALLD
    EXPECT_LT(ci_width1, ci_width2);
}

// ==================== Strategy Noise Resistance Tests ====================

TEST_F(NoiseTest, TFT_DegradedByNoise) {
    GameConfig::GameConfig::repeats = 20;
    std::vector<StrategyTypes> strategies = {StrategyTypes::TFT};
    
    // No noise
    GameConfig::GameConfig::epsilon = 0.0;
    Tournament::Tournament tournament1(strategies);
    tournament1.simulateTournament();
    double noNoiseMean = tournament1.getMatchResults()[0].p1Mean;
    
    // With noise
    GameConfig::GameConfig::epsilon = 0.1;
    Tournament::Tournament tournament2(strategies);
    tournament2.simulateTournament();
    double noiseMean = tournament2.getMatchResults()[0].p1Mean;
    
    // TFT vs TFT should score lower with noise
    EXPECT_LT(noiseMean, noNoiseMean);
}

TEST_F(NoiseTest, GRIM_VulnerableToNoise) {
    GameConfig::GameConfig::repeats = 20;
    std::vector<StrategyTypes> strategies = {StrategyTypes::GRIM};
    
    // No noise - should get perfect cooperation
    GameConfig::GameConfig::epsilon = 0.0;
    Tournament::Tournament tournament1(strategies);
    tournament1.simulateTournament();
    double noNoiseMean = tournament1.getMatchResults()[0].p1Mean;
    
    // With noise - cooperation breaks down
    GameConfig::GameConfig::epsilon = 0.05;
    Tournament::Tournament tournament2(strategies);
    tournament2.simulateTournament();
    double noiseMean = tournament2.getMatchResults()[0].p1Mean;
    
    // GRIM should be significantly hurt by noise
    EXPECT_LT(noiseMean, noNoiseMean * 0.9); // At least 10% degradation
}

TEST_F(NoiseTest, PAVLOV_BetterOverallPerformanceInNoise) {
    GameConfig::GameConfig::epsilon = 0.08;
    GameConfig::GameConfig::repeats = 30;

    std::vector strategies = {
        StrategyTypes::ALLC,
        StrategyTypes::ALLD,
        StrategyTypes::TFT,
        StrategyTypes::GRIM,
        StrategyTypes::PAVLOV
    };

    Tournament::Tournament tournament(strategies);
    tournament.simulateTournament();

    auto& totalScores = tournament.getTotalPayoffs();

    // Calculate mean scores across all matches
    double tftMean = MathUtil::calculateMean(
        totalScores[StrategyTypes::TFT],
        totalScores[StrategyTypes::TFT].size()
    );

    double pavlovMean = MathUtil::calculateMean(
        totalScores[StrategyTypes::PAVLOV],
        totalScores[StrategyTypes::PAVLOV].size()
    );

    // In a noisy environment against diverse opponents,
    // PAVLOV should outperform TFT
    EXPECT_GE(pavlovMean, tftMean);
}

TEST_F(NoiseTest, CONTRITE_RobustToNoise) {
    GameConfig::GameConfig::epsilon = 0.05;
    GameConfig::GameConfig::repeats = 30;
    
    std::vector<StrategyTypes> strategies = {StrategyTypes::CONTRITE};
    
    Tournament::Tournament tournament(strategies);
    tournament.simulateTournament();
    
    auto& matches = tournament.getMatchResults();
    double contriteMean = matches[0].p1Mean;
    
    // Expected payoff with perfect cooperation (R per round)
    double expectedPerfect = 3.0 * GameConfig::GameConfig::rounds;
    
    // CONTRITE should maintain high cooperation despite noise
    EXPECT_GT(contriteMean, expectedPerfect * 0.85); // At least 85% of perfect
}

// ==================== Noise Level Sweep Tests ====================

TEST_F(NoiseTest, PerformanceDegradesWithIncreasingNoise) {
    GameConfig::GameConfig::repeats = 10;
    std::vector<StrategyTypes> strategies = {StrategyTypes::TFT};
    
    std::vector<double> noiselevels = {0.0, 0.02, 0.05, 0.1};
    std::vector<double> scores;
    
    for (double epsilon : noiselevels) {
        GameConfig::GameConfig::epsilon = epsilon;
        Tournament::Tournament tournament(strategies);
        tournament.simulateTournament();
        scores.push_back(tournament.getMatchResults()[0].p1Mean);
    }
    
    // Scores should generally decrease with more noise
    for (size_t i = 1; i < scores.size(); ++i) {
        EXPECT_LE(scores[i], scores[i-1] * 1.05); // Allow 5% variance
    }
}
