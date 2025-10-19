#include "../../include/StrategyAgents/ALLD.h"

#include <iostream>

namespace StrategyAgents {

    ResponseType ALLD::decide(const ResponseType& lastResponse) { return noisyResponse(ResponseType::D); }

}