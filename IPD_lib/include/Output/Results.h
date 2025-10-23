#pragma once

#include "../Tournament.h"
#include "../EvolutionaryTournament.h"

namespace Tournament::Evolution {
    class EvolutionaryTournament;
}

namespace Results {
    inline static int defaultWidth = 15;
    inline static int defaultPrecision = 4;
    inline static std::string divider = "------------------------------------------------------------";

    class Output {
    public:
        virtual ~Output() = default;
        explicit Output(std::unique_ptr<Tournament::Tournament>&& tournament) : tournament(std::move(tournament)) {}

        virtual std::ostream& logResults(std::ostream& os) {return os;}

        friend std::ostream& operator<<(std::ostream& os, Output& output) { return output.logResults(os); }

    protected:
        std::unique_ptr<Tournament::Tournament> tournament;

    };

    namespace Evolution {

        class Output {
        public:
            virtual ~Output() = default;
            Output(std::unique_ptr<Tournament::Evolution::EvolutionaryTournament>&& tournament, const int population, const int generations, const double mutation, const bool useSCB) :
               tournament(std::move(tournament)), population(population), generations(generations), mutation(mutation), useSCB(useSCB) {}

            virtual std::ostream& logResults(std::ostream& os) {return os;}

            friend std::ostream& operator<<(std::ostream& os, Output& output) { return output.logResults(os); }
        protected:
            std::unique_ptr<Tournament::Evolution::EvolutionaryTournament> tournament;
            const int population; const int generations; const double mutation;

            bool useSCB;
        };
    }
}
