#pragma once

#include <vector>

namespace MathUtil {

        double calculateMean(const std::vector<double>& data, int rpt);
        double calculateStdDev(const std::vector<double>& data, int rpt, double mean);
        double calculateStdDev(const std::vector<double>& data, int rpt);
        double calculateStdErr(double stdDev, int rpt);
        std::pair<double,double> calculateCI(const std::vector<double>& data, int rpt);
        std::pair<double,double> calculateCI(double mean, double stdDev, int rpt);

}
