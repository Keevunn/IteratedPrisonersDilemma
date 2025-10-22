#include <gtest/gtest.h>
#include "../include/Tournament.h"
#include "../include/GameConfig.h"

using namespace GameConfig::Strategies;

class TournamentTest : public ::testing::Test {
protected:
    void SetUp() override {
        GameConfig::GameConfig::rounds = 5;
        GameConfig::GameConfig::repeats = 10;
        GameConfig::GameConfig::seed = 42;
        GameConfig::GameConfig::epsilon = 0.0;
        GameConfig::GameConfig::payoffs = {5, 3, 1, 0};
    }
};

// ==================== Tournament Structure Tests ====================

TEST_F(TournamentTest, Tournament_CreatesCorrectNumberOfMatches) {
    std::vector<StrategyTypes> strategies = {
        StrategyTypes::ALLC,
        StrategyTypes::ALLD,
        StrategyTypes::TFT
    };
    
    Tournament::Tournament tournament(strategies);
    tournament.simulateTournament();
    
    // 3 strategies × 3 strategies = 9 matches
    EXPECT_EQ(tournament.getMatchResults().size(), 9);
}

TEST_F(TournamentTest, Tournament_TwoStrategies) {
    std::vector<StrategyTypes> strategies = {
        StrategyTypes::ALLC,
        StrategyTypes::ALLD
    };
    
    Tournament::Tournament tournament(strategies);
    tournament.simulateTournament();
    
    // 2 strategies × 2 strategies = 4 matches
    EXPECT_EQ(tournament.getMatchResults().size(), 4);
}

TEST_F(TournamentTest, Tournament_SingleStrategy) {
    std::vector<StrategyTypes> strategies = {StrategyTypes::ALLC};
    
    Tournament::Tournament tournament(strategies);
    tournament.simulateTournament();
    
    // 1 strategy × 1 strategy = 1 match (self-play)
    EXPECT_EQ(tournament.getMatchResults().size(), 1);
}

// ==================== Score Tracking Tests ====================

TEST_F(TournamentTest, TotalScores_TrackAllRepeats) {
    std::vector<StrategyTypes> strategies = {
        StrategyTypes::ALLC,
        StrategyTypes::ALLD
    };
    
    Tournament::Tournament tournament(strategies);
    tournament.simulateTournament();
    
    auto& scores = tournament.getTotalScores();
    
    // Each strategy plays 4 matches × 10 repeats = 40 scores
    // ALLC vs ALLC, ALLC vs ALLD, ALLD vs ALLC, ALLD vs ALLD
    EXPECT_EQ(scores[StrategyTypes::ALLC].size(), 40);
    EXPECT_EQ(scores[StrategyTypes::ALLD].size(), 40);
}

TEST_F(TournamentTest, TotalScores_AllPositiveOrZero) {
    std::vector<StrategyTypes> strategies = {
        StrategyTypes::ALLC,
        StrategyTypes::ALLD,
        StrategyTypes::TFT
    };
    
    Tournament::Tournament tournament(strategies);
    tournament.simulateTournament();
    
    auto& scores = tournament.getTotalScores();
    
    for (const auto& [strat, scoreList] : scores) {
        for (double score : scoreList) {
            EXPECT_GE(score, 0.0);
        }
    }
}

// ==================== Match Results Tests ====================

TEST_F(TournamentTest, MatchResults_HaveValidMeans) {
    std::vector<StrategyTypes> strategies = {
        StrategyTypes::ALLC,
        StrategyTypes::ALLD
    };
    
    Tournament::Tournament tournament(strategies);
    tournament.simulateTournament();
    
    auto& matches = tournament.getMatchResults();
    
    for (const auto& match : matches) {
        EXPECT_GE(match.p1Mean, 0.0);
        EXPECT_GE(match.p2Mean, 0.0);
        EXPECT_LE(match.p1Mean, 5.0 * GameConfig::GameConfig::rounds); // Max T × rounds
        EXPECT_LE(match.p2Mean, 5.0 * GameConfig::GameConfig::rounds);
    }
}

TEST_F(TournamentTest, MatchResults_HaveValidConfidenceIntervals) {
    std::vector<StrategyTypes> strategies = {
        StrategyTypes::ALLC,
        StrategyTypes::ALLD
    };
    
    Tournament::Tournament tournament(strategies);
    tournament.simulateTournament();
    
    auto& matches = tournament.getMatchResults();
    
    for (const auto& match : matches) {
        // Lower bound should be less than mean
        EXPECT_LE(match.p1CI.first, match.p1Mean);
        EXPECT_LE(match.p2CI.first, match.p2Mean);
        
        // Upper bound should be greater than mean
        EXPECT_GE(match.p1CI.second, match.p1Mean);
        EXPECT_GE(match.p2CI.second, match.p2Mean);
    }
}

// ==================== Expected Outcomes Tests ====================

TEST_F(TournamentTest, ALLC_vs_ALLC_ExpectedScore) {
    GameConfig::GameConfig::rounds = 5;
    GameConfig::GameConfig::repeats = 1;
    
    std::vector<StrategyTypes> strategies = {StrategyTypes::ALLC};
    
    Tournament::Tournament tournament(strategies);
    tournament.simulateTournament();
    
    auto& matches = tournament.getMatchResults();
    
    // ALLC vs ALLC: 5 rounds × 3 (R) = 15
    EXPECT_NEAR(matches[0].p1Mean, 15.0, 0.01);
    EXPECT_NEAR(matches[0].p2Mean, 15.0, 0.01);
}

TEST_F(TournamentTest, ALLD_vs_ALLD_ExpectedScore) {
    GameConfig::GameConfig::rounds = 5;
    GameConfig::GameConfig::repeats = 1;
    
    std::vector<StrategyTypes> strategies = {StrategyTypes::ALLD};
    
    Tournament::Tournament tournament(strategies);
    tournament.simulateTournament();
    
    auto& matches = tournament.getMatchResults();
    
    // ALLD vs ALLD: 5 rounds × 1 (P) = 5
    EXPECT_NEAR(matches[0].p1Mean, 5.0, 0.01);
    EXPECT_NEAR(matches[0].p2Mean, 5.0, 0.01);
}

TEST_F(TournamentTest, ALLC_vs_ALLD_Asymmetric) {
    GameConfig::GameConfig::rounds = 5;
    GameConfig::GameConfig::repeats = 1;
    
    std::vector<StrategyTypes> strategies = {
        StrategyTypes::ALLC,
        StrategyTypes::ALLD
    };
    
    Tournament::Tournament tournament(strategies);
    tournament.simulateTournament();
    
    auto& matches = tournament.getMatchResults();
    
    // Find ALLC vs ALLD match
    for (const auto& match : matches) {
        if (match.player1->getStrategy() == StrategyTypes::ALLC &&
            match.player2->getStrategy() == StrategyTypes::ALLD) {
            // ALLC gets 0, ALLD gets 25
            EXPECT_NEAR(match.p1Mean, 0.0, 0.01);
            EXPECT_NEAR(match.p2Mean, 25.0, 0.01);
        }
    }
}

// ==================== Deterministic Tests ====================

TEST_F(TournamentTest, SameSeed_ProducesSameResults) {
    GameConfig::GameConfig::seed = 123;
    
    std::vector<StrategyTypes> strategies = {
        StrategyTypes::ALLC,
        StrategyTypes::ALLD,
        StrategyTypes::TFT
    };
    
    Tournament::Tournament tournament1(strategies);
    tournament1.simulateTournament();
    auto scores1 = tournament1.getTotalScores();
    
    // Reset seed
    GameConfig::GameConfig::seed = 123;
    
    Tournament::Tournament tournament2(strategies);
    tournament2.simulateTournament();
    auto scores2 = tournament2.getTotalScores();
    
    // Results should be identical
    EXPECT_EQ(scores1[StrategyTypes::ALLC].size(), scores2[StrategyTypes::ALLC].size());
    EXPECT_EQ(scores1[StrategyTypes::ALLD].size(), scores2[StrategyTypes::ALLD].size());
    EXPECT_EQ(scores1[StrategyTypes::TFT].size(), scores2[StrategyTypes::TFT].size());
}
