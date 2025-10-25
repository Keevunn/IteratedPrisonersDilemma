#pragma once

#include <filesystem>
#include <string_view>
#include <vector>

#include "EvolutionaryTournament.h"
#include "GameConfig.h"
#include "RandomUtil.h"
#include "Tournament.h"

using namespace GameConfig::Strategies;

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
            Random::Random::seed(GameConfig::GameConfig::seed);
            if (shouldEvolve)
                tournament = std::make_unique<Tournament::Evolution::EvolutionaryTournament>(strategies, population, generations, mutation, enableSCB);
            else
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
        std::filesystem::path findConfigFile(const std::string_view& fileName);

        std::ofstream& outputResults(std::ofstream& file);
        std::filesystem::path generateFileName();

        // Configuration data
        std::vector<StrategyTypes> strategies;
        std::string format = "text";

        // Flags
        bool shouldEvolve = false;
        bool enableSCB = false;

        // Evolution params (only if shouldEvolve)
        int population = 100;
        int generations = 50;
        double mutation = 0.00;

        // Simulation data
        std::unique_ptr<Tournament::Tournament> tournament;

        inline static std::filesystem::path ROOT = OUTPUT_DIR;
    };
}
