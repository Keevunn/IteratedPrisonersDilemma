#include "../../include/StrategyAgents/CTFT.h"

#include <iostream>

namespace StrategyAgents {

    std::string CTFT::decide() {
        //
    }

    std::string CTFT::decide(const std::string_view &lastResponse) {
        //
    }

    std::ostream& operator<<(std::ostream& os, const CTFT& agent) {
        os << std::string("CTFT");
        return os;
    }


}