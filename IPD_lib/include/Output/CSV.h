#pragma once
#include "Output.h"

namespace Output::CSV {
    class CSV : public Output {
    public:
        explicit CSV(std::unique_ptr<Tournament::Tournament>& tournament) : Output(tournament) {}

        std::ostream& results(std::ostream& os) override;

    };
}