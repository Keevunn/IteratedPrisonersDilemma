#pragma once
#include <iosfwd>

namespace Tournament {

    // For encapsulating the match results AFTER repeats
    class Results {
    public:
        Results() = default;

        void setMean(double p1Mean, double p2Mean);
        void setStdDev(double p1StdDev, double p2StdDev);

        int repeatNum = 0;

        friend std::ostream& operator<<(std::ostream& os, const Results& results);
    private:

        struct MeanStruct {
            double p1Mean{}; double p2Mean{};
        } mean;
        struct stdDevStruct {
            double p1StdDev{}; double p2StdDev{};
        } stdDev;

    };

}
