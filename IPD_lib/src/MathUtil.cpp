#include "../include/MathUtil.h"

#include <numeric>
#include <cmath>

namespace MathUtil {

#define NORMAL_DIST_CONSTANT 1.96

    double calculateMean(const std::vector<double>& data, const int size) {
        double sum = std::accumulate(data.begin(), data.end(), 0.0);
        return sum / size;
    }

    double calculateStdDev(const std::vector<double>& data, const int size, const double mean){
        const double numerator = std::accumulate(data.begin(), data.end(), 0.0,
            [&mean](const double total, const double x)-> double { return total + std::pow(x - mean, 2); });

        return std::sqrt(numerator / size);
    }

    double calculateStdDev(const std::vector<double>& data, const int size) {
        const double mean = calculateMean(data, size);

        return calculateStdDev(data, size, mean);
    }

    double calculateStdErr(const double stdDev, const int size) {
        return stdDev / std::sqrt(size);
    }

    std::pair<double,double> calculateCI(const std::vector<double>& data, const int size) {
        double mean = calculateMean(data, size);
        double stdDev = calculateStdDev(data, size, mean);

        return calculateCI(mean, stdDev, size);
    }

    std::pair<double,double> calculateCI(const double mean, const double stdDev, const int size) {
        const double critReg = NORMAL_DIST_CONSTANT * calculateStdErr(stdDev, size);
        return {mean-critReg, mean + critReg};
    }

}
