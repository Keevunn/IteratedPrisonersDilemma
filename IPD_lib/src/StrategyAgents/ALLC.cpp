#include "../../include/StrategyAgents/ALLC.h"

namespace StrategyAgents {

    ResponseType ALLC::decide(const ResponseType& lastResponse) { return noisyResponse(ResponseType::C); }

}