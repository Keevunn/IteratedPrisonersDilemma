#pragma once

#include "../Tournament.h"

namespace Output {
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
        Output(std::unique_ptr<Tournament::Tournament>& tournament) : tournament(tournament) {}

        virtual std::ostream& results(std::ostream& os);

        friend std::ostream& operator<<(std::ostream& os, Output& output);

    protected:
        [[nodiscard]] std::vector<Statistics::OverallStats> generateLeaderboard() const;
        [[nodiscard]] std::unordered_map<StrategyTypes, std::vector<double>> generatePayoffMatrix() const;

        std::unique_ptr<Tournament::Tournament>& tournament;

    };
}
