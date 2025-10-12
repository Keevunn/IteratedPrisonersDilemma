#pragma once
#include <array>
#include <charconv>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>


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
        enum class ArgTypes{
            ROUNDS, REPEATS, SEED, EPSILON, PAYOFFS,
            STRATEGIES, FORMAT, SAVE, LOAD,
            EVOLVE, POPULATION, GENERATIONS, MUTATION
        };

         inline static const std::unordered_map<std::string_view, ArgTypes> argMap = {
            {"--rounds", ArgTypes::ROUNDS},
            {"--repeats", ArgTypes::REPEATS},
            {"--epsilon", ArgTypes::EPSILON},
            {"--seed", ArgTypes::SEED},
            {"--payoffs", ArgTypes::PAYOFFS},
            {"--strategies", ArgTypes::STRATEGIES},
            {"--format", ArgTypes::FORMAT},
            {"--save", ArgTypes::SAVE},
            {"--load", ArgTypes::LOAD},
            {"--evolve", ArgTypes::EVOLVE},
            {"--population", ArgTypes::POPULATION},
            {"--generations", ArgTypes::GENERATIONS},
            {"--mutation", ArgTypes::MUTATION}
        };

        enum class StrategyTypes{
            ALLC,ALLD,TFT,GRIM,PAVLOV,RND03,CONTRITE,PROBER
        };

        inline static const std::unordered_map<std::string_view, StrategyTypes> strategyMap = {
            {"ALLC", StrategyTypes::ALLC},
            {"ALLD", StrategyTypes::ALLD},
            {"TFT", StrategyTypes::TFT},
            {"GRIM", StrategyTypes::GRIM},
            {"PAVLOV", StrategyTypes::PAVLOV},
            {"RNDO.3", StrategyTypes::RND03},
            {"CONTRITE", StrategyTypes::CONTRITE},
            {"PROBER", StrategyTypes::PROBER}
        };

        void parseArgs(const std::vector<std::string_view>& args);
        static ArgTypes validateArg(std::string_view arg);
        static void validateFormat(std::string_view format);
        void validatePayoffs(const std::string_view& param);
        void parseStrategies(const std::string_view& param);
        static StrategyTypes validateStrategies(const std::string_view& strategy);
        static void validateFileName(const std::string_view& fileName);
        static void saveConfig(const std::string_view& fileName, const std::vector<std::string_view>& args);
        void loadConfig(const std::string_view& fileName);


        template<typename T> requires std::is_arithmetic_v<T>
        T parseParam(const std::string_view& param, const std::string&& arg) const{
            auto first = param.data(); auto last = param.data()+param.size();
            T value{};

            if (auto res = std::from_chars(first, last, value); res.ec != std::errc() || res.ptr != last)
                throw std::invalid_argument("Invalid parameter for " + arg + " argument: " + std::string(param));
            return value;
        }

        // Configuration data
        int rounds = 100;
        int repeats = 1;
        int seed{};
        float epsilon{};

        std::string format = "text";
        std::array<float, 4> payoffs = {5, 3, 1, 0};
        std::vector<StrategyTypes> strategies;

        // Flags
        bool shouldEvolve = false;

        // Evolution params (only if shouldEvolve)
        int population = 100;
        int generations = 50;
        float mutation = 0.01;

};
