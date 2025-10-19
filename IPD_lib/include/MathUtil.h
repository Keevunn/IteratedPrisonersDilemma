#pragma once

#include <vector>

namespace MathUtil {

        double calculateMean(const std::vector<double>& data, int size);
        double calculateStdDev(const std::vector<double>& data, int size, double mean);
        double calculateStdDev(const std::vector<double>& data, int size);
        double calculateStdErr(double stdDev, int size);
        std::pair<double,double> calculateCI(const std::vector<double>& data, int size);
        std::pair<double,double> calculateCI(double mean, double stdDev, int size);

}
