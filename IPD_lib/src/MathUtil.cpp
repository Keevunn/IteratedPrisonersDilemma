#include "../include/MathUtil.h"

#include <numeric>
#include <cmath>

namespace MathUtil {

#define NORMAL_DIST_CONSTANT 1.96

    double Maths::calculateMean(std::vector<double>& data, double rpt) {
        double sum = std::accumulate(data.begin(), data.end(), 0.0);
        return sum / rpt;
    }

    double Maths::calculateStdDev(std::vector<double>& data, int rpt, double mean){
        double numerator = std::accumulate(data.begin(), data.end(), 0.0,
            [&mean](int total, int x)-> double { return total + std::pow(x - mean, 2); });

        return std::sqrt(numerator / rpt);
    }

    double Maths::calculateStdDev(std::vector<double>& data, int rpt) {
        double mean = calculateMean(data, rpt);

        return calculateStdDev(data, rpt, mean);
    }

    double Maths::calculateStdErr(double stdDev, int rpt) {
        return stdDev / std::sqrt(rpt);
    }

    std::pair<double,double> Maths::calculateCI(std::vector<double>& data, int rpt) {
        double mean = calculateMean(data, rpt);
        double stdDev = calculateStdDev(data, rpt, mean);

        return calculateCI(mean, stdDev, rpt);
    }

    std::pair<double,double> Maths::calculateCI(double mean, double stdDev, int rpt) {
        double critReg = NORMAL_DIST_CONSTANT * calculateStdErr(stdDev, rpt);

        return {mean - critReg, mean + critReg};
    }

}
