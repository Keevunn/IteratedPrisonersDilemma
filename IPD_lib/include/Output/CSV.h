#pragma once
#include "Output.h"

namespace Output::CSV {
    class CSV : public Output {
    public:
        explicit CSV(std::unique_ptr<Tournament::Tournament>& tournament) : Output(tournament) {}

        std::ostream& results(std::ostream& os) override;

    };

    namespace Evolution {
        class Evolution : public CSV {
        public:
            explicit Evolution(std::unique_ptr<Tournament::Tournament>& tournament, const int population, const int generations, const int mutation) :
                CSV(tournament), population(population), generations(generations), mutation(mutation) {}

            std::ostream& results(std::ostream& os) override;

        private:
            int population; int generations; double mutation;
        };
    }
}