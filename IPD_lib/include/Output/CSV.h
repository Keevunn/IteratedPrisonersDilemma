#pragma once
#include "Results.h"

namespace Results::CSV {
    class Output : public Results::Output {
    public:
        explicit Output(std::unique_ptr<Tournament::Tournament>&& tournament) : Results::Output(std::move(tournament)) {}

        std::ostream& logResults(std::ostream& os) override;

    };

    namespace Evolution {
        class Output : public Results::Evolution::Output {
        public:
             Output(std::unique_ptr<Tournament::Evolution::EvolutionaryTournament>&& tournament, const int population, const int generations, const double mutation, const bool useSCB) :
                Results::Evolution::Output(std::move(tournament), population, generations, mutation, useSCB) {}

            std::ostream& logResults(std::ostream& os) override;

        };
    }
}