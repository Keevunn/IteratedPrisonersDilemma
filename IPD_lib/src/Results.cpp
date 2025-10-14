#include "../include/Results.h"

#include <string>
#include <sstream>

namespace Tournament {

    void Results::setMean(double p1Mean, double p2Mean) {
        mean.p1Mean = p1Mean;
        mean.p2Mean = p2Mean;
    }
    void Results::setStdDev(double p1StdDev, double p2StdDev) {
        stdDev.p1StdDev = p1StdDev;
        stdDev.p2StdDev = p2StdDev;
    }

    std::ostream& operator<<(std::ostream& os, const Results& results) {
        std::string outputStr =
"       | Player1 | Player2 | "
"Mean   | 5.3     |   5.3   | "
"StdDev | 5.3     |   5.3   | ";
        os << outputStr;
    }
}
