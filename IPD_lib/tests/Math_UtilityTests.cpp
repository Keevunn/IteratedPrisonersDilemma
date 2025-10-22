#include <gtest/gtest.h>
#include "../include/MathUtil.h"
#include <cmath>

using namespace MathUtil;

class MathTest : public ::testing::Test {
protected:
    const double TOLERANCE = 1e-6;
};

// ==================== Mean Tests ====================

TEST_F(MathTest, CalculateMean_SimpleValues) {
    std::vector<double> data = {1.0, 2.0, 3.0, 4.0, 5.0};
    EXPECT_NEAR(calculateMean(data, 5), 3.0, TOLERANCE);
}

TEST_F(MathTest, CalculateMean_AllSame) {
    std::vector<double> data = {5.0, 5.0, 5.0, 5.0};
    EXPECT_NEAR(calculateMean(data, 4), 5.0, TOLERANCE);
}

TEST_F(MathTest, CalculateMean_WithDecimals) {
    std::vector<double> data = {1.5, 2.5, 3.5};
    EXPECT_NEAR(calculateMean(data, 3), 2.5, TOLERANCE);
}

TEST_F(MathTest, CalculateMean_SingleValue) {
    std::vector<double> data = {42.0};
    EXPECT_NEAR(calculateMean(data, 1), 42.0, TOLERANCE);
}

// ==================== Standard Deviation Tests ====================

TEST_F(MathTest, CalculateStdDev_NoVariance) {
    std::vector<double> data = {5.0, 5.0, 5.0, 5.0};
    EXPECT_NEAR(calculateStdDev(data, 4), 0.0, TOLERANCE);
}

TEST_F(MathTest, CalculateStdDev_SimpleVariance) {
    std::vector<double> data = {2.0, 4.0, 4.0, 4.0, 5.0, 5.0, 7.0, 9.0};
    double mean = calculateMean(data, 8);
    double stdDev = calculateStdDev(data, 8, mean);
    EXPECT_NEAR(stdDev, 2.0, TOLERANCE);
}

TEST_F(MathTest, CalculateStdDev_WithMeanProvided) {
    std::vector<double> data = {1.0, 2.0, 3.0, 4.0, 5.0};
    double mean = 3.0;
    double stdDev = calculateStdDev(data, 5, mean);
    EXPECT_NEAR(stdDev, std::sqrt(2.0), TOLERANCE);
}

TEST_F(MathTest, CalculateStdDev_WithoutMeanProvided) {
    std::vector<double> data = {1.0, 2.0, 3.0, 4.0, 5.0};
    double stdDev = calculateStdDev(data, 5);
    EXPECT_NEAR(stdDev, std::sqrt(2.0), TOLERANCE);
}

// ==================== Standard Error Tests ====================

TEST_F(MathTest, CalculateStdErr_SimpleCase) {
    double stdDev = 10.0;
    int size = 100;
    EXPECT_NEAR(calculateStdErr(stdDev, size), 1.0, TOLERANCE);
}

TEST_F(MathTest, CalculateStdErr_SmallSample) {
    double stdDev = 2.0;
    int size = 4;
    EXPECT_NEAR(calculateStdErr(stdDev, size), 1.0, TOLERANCE);
}

// ==================== Confidence Interval Tests ====================

TEST_F(MathTest, CalculateCI_SimpleCase) {
    std::vector<double> data = {100.0, 100.0, 100.0, 100.0};
    auto [lower, upper] = calculateCI(data, 4);
    
    // With zero variance, CI should be tight around mean
    EXPECT_NEAR(lower, 100.0, TOLERANCE);
    EXPECT_NEAR(upper, 100.0, TOLERANCE);
}

TEST_F(MathTest, CalculateCI_WithVariance) {
    double mean = 50.0;
    double stdDev = 10.0;
    int size = 100;
    
    auto [lower, upper] = calculateCI(mean, stdDev, size);
    
    // 95% CI: mean ± 1.96 * (stdDev / sqrt(size))
    // = 50 ± 1.96 * (10 / 10) = 50 ± 1.96
    EXPECT_NEAR(lower, 48.04, 0.01);
    EXPECT_NEAR(upper, 51.96, 0.01);
}

TEST_F(MathTest, CalculateCI_LargerSampleTighterInterval) {
    double mean = 50.0;
    double stdDev = 10.0;
    
    auto [lower1, upper1] = calculateCI(mean, stdDev, 100);
    auto [lower2, upper2] = calculateCI(mean, stdDev, 400);
    
    double width1 = upper1 - lower1;
    double width2 = upper2 - lower2;
    
    // Larger sample should have tighter CI
    EXPECT_LT(width2, width1);
}

// ==================== Integration Tests ====================

TEST_F(MathTest, FullPipeline_MeanStdDevCI) {
    std::vector<double> data = {95.0, 100.0, 105.0, 100.0, 100.0};
    int size = 5;
    
    double mean = calculateMean(data, size);
    EXPECT_NEAR(mean, 100.0, TOLERANCE);
    
    double stdDev = calculateStdDev(data, size, mean);
    EXPECT_GT(stdDev, 0.0);
    
    auto [lower, upper] = calculateCI(mean, stdDev, size);
    EXPECT_LT(lower, mean);
    EXPECT_GT(upper, mean);
}
