#include "../include/MathUtil.h"

#include <numeric>
#include <cmath>

namespace MathUtil {

#define NORMAL_DIST_CONSTANT 1.96

    double calculateMean(const std::vector<double>& data, int rpt) {
        double sum = std::accumulate(data.begin(), data.end(), 0.0);
        return sum / rpt;
    }

    double calculateStdDev(const std::vector<double>& data, const int rpt, const double mean){
        const double numerator = std::accumulate(data.begin(), data.end(), 0.0,
            [&mean](const double total, const double x)-> double { return total + std::pow(x - mean, 2); });

        return std::sqrt(numerator / rpt);
    }

    double calculateStdDev(const std::vector<double>& data, const int rpt) {
        const double mean = calculateMean(data, rpt);

        return calculateStdDev(data, rpt, mean);
    }

    double calculateStdErr(const double stdDev, const int rpt) {
        return stdDev / std::sqrt(rpt);
    }

    std::pair<double,double> calculateCI(const std::vector<double>& data, const int rpt) {
        double mean = calculateMean(data, rpt);
        double stdDev = calculateStdDev(data, rpt, mean);

        return calculateCI(mean, stdDev, rpt);
    }

    std::pair<double,double> calculateCI(const double mean, const double stdDev, const int rpt) {
        double critReg = NORMAL_DIST_CONSTANT * calculateStdErr(stdDev, rpt);

        return {mean - critReg, mean + critReg};
    }

}
