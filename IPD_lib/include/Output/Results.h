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

        virtual std::ostream& logResults(std::ostream& os);

        friend std::ostream& operator<<(std::ostream& os, Output& output);

    protected:
        [[nodiscard]] static std::vector<Statistics::OverallStats> generateStats(const std::unordered_map<StrategyTypes, std::vector<double>>& data) ;
        [[nodiscard]] std::unordered_map<StrategyTypes, std::vector<double>> generatePayoffMatrix() const;

        std::unique_ptr<Tournament::Tournament> tournament;

    };
}
