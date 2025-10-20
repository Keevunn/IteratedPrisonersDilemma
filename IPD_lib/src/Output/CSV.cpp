#include "../../include/Output/CSV.h"

namespace Output::CSV {
    std::ostream &CSV::results(std::ostream &os) {
        os << "Strategy, Opponent, Mean, StdDev, CI_LB, CI_UB, Rounds, Repeats, Seed, Epsilon, T, R, P, S" << std::endl;
        for (const auto& match : tournament->getMatchResults())
            os << match;
        return os;
    }

}