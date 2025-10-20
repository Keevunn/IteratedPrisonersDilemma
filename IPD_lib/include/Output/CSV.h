#pragma once
#include "Results.h"

namespace Results::CSV {
    class Output : public ::Results::Output {
    public:
        explicit CSV(std::unique_ptr<Tournament::Tournament>& tournament) : Output(tournament) {}

        std::ostream& logResults(std::ostream& os) override;

    };

    namespace Evolution {
        class Evolution : public CSV {
        public:
            explicit Evolution(std::unique_ptr<Tournament::Tournament>& tournament, const int population, const int generations, const int mutation) :
                CSV(tournament), population(population), generations(generations), mutation(mutation) {}

            std::ostream& logResults(std::ostream& os) override;

        private:
            int population; int generations; double mutation;
        };
    }
}