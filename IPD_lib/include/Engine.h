#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "GameConfig.h"

using namespace GameConfig::Strategies;
using namespace GameConfig::Args;

namespace Engine {



    class Engine {
    public:
        Engine(int argc, char* argv[]) {
            std::vector<std::string_view> args(argv + 1, argv + argc);
            parseArgs(args);
        };
        explicit Engine(const std::vector<std::string_view>& args) {
            parseArgs(args);
        }
        Engine() = default;

    private:

        void parseArgs(const std::vector<std::string_view>& args);
        static void saveConfig(const std::string_view& fileName, const std::vector<std::string_view>& args);
        void loadConfig(const std::string_view& fileName);

        // Configuration data
        std::vector<StrategyTypes> strategies;
        std::string format = "text";

        // Flags
        bool shouldEvolve = false;

        // Evolution params (only if shouldEvolve)
        int population = 100;
        int generations = 50;
        double mutation = 0.01;

    };
}
