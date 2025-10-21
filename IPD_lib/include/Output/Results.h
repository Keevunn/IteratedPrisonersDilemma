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

    namespace Statistics {
        typedef struct OverallStatsStruct {
            OverallStatsStruct() = default;

            std::string name{};
            double mean{};
            double stdDev{};
            std::pair<double, double> CI{};

            friend std::ostream& operator<<(std::ostream& os, const OverallStatsStruct& stats);
        } OverallStats;
    }

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
            Output(std::unique_ptr<Tournament::Evolution::EvolutionaryTournament>&& tournament, const int population, const int generations, const double mutation) :
               tournament(std::move(tournament)), population(population), generations(generations), mutation(mutation) {}

            virtual std::ostream& logResults(std::ostream& os) {return os;}

            friend std::ostream& operator<<(std::ostream& os, Output& output) { return output.logResults(os); }
        protected:
            std::unique_ptr<Tournament::Evolution::EvolutionaryTournament> tournament;
            const int population; const int generations; const double mutation;

        };
    }
}
