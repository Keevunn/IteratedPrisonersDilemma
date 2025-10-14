#pragma once

#include <vector>

namespace MathUtil {

    class Maths {
    public:
        static double calculateMean(std::vector<double>& data, double rpt);
        static double calculateStdDev(std::vector<double>& data, int rpt, double mean);
        static double calculateStdDev(std::vector<double>& data, int rpt);
        static double calculateStdErr(double stdDev, int rpt);
        static std::pair<double,double> calculateCI(std::vector<double>& data, int rpt);
        static std::pair<double,double> calculateCI(double mean, double stdDev, int rpt);

    };

}
