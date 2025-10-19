#pragma once

#include <string_view>
#include <vector>

#include "GameConfig.h"
#include "Tournament.h"

using namespace GameConfig::Strategies;
using namespace GameConfig::Args;

namespace Engine {

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

    class Engine {
    public:
        Engine(const int argc, char* argv[]) {
            const std::vector<std::string_view> args(argv + 1, argv + argc);
            parseArgs(args);
            tournament = std::make_unique<Tournament::Tournament>(strategies);
        };
        explicit Engine(const std::vector<std::string_view>& args) { // For testing purposes
            parseArgs(args);
            tournament = std::make_unique<Tournament::Tournament>(strategies);
        }

        void runIPD();

    private:

        void parseArgs(const std::vector<std::string_view>& args);
        void saveConfig(const std::string_view& fileName, const std::vector<std::string_view>& args);
        void loadConfig(const std::string_view& fileName);
        // Output functions
        void outputResults();
        std::ostream& outputText(std::ostream& os);
        std::unique_ptr<std::vector<Statistics::OverallStats>> generateLeaderboard() const;
        std::ostream& outputLeaderboard(std::ostream& os) const;

        std::ostream& outputCSV(std::ostream& os);
        std::ostream& outputPayoffMatrix(std::ostream& os);

        std::ostream& outputJSON(std::ostream& os);

        // Configuration data
        std::vector<StrategyTypes> strategies;

        // Flags
        bool shouldEvolve = false;

        // Evolution params (only if shouldEvolve)
        int population = 100;
        int generations = 50;
        double mutation = 0.01;

        // Simulation data
        std::unique_ptr<Tournament::Tournament> tournament;

    };
}
