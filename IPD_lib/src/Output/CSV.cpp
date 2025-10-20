#include "../../include/Output/CSV.h"

namespace Results::CSV {
    std::ostream &CSV::logResults(std::ostream &os) {
        os << "Strategy, Opponent, Mean, StdDev, CI_LB, CI_UB, Rounds, Repeats, Seed, Epsilon, T, R, P, S" << std::endl;
        for (const auto& match : tournament->getMatchResults())
            os << match;
        return os;
    }

}