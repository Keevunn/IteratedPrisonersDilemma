#pragma once
#include <array>
#include <charconv>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace Engine {
    class Engine {
    public:
        Engine(int argc, char* argv[]) {parseArgs(argc, argv);};
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

        void parseArgs(int argc, char* argv[]);
        static void validateFormat(std::string_view format);
        void validatePayoffs(const std::string_view& param) const; // TODO Use formula T > R > P > S and 2R > T + S
        //void validateStrategies(); // TODO Use std::map ...
        static ArgTypes validateArg(std::string_view arg); // TODO Use std::map and enum for parsing of flags + validation

        template<typename T> requires std::is_arithmetic_v<T>
        T parseParam(const std::string_view& param, const std::string&& arg) const{
            auto first = param.begin(); auto last = param.end();
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
        std::string fileName;
        std::array<float, 4> payoffs = {5, 3, 1, 0};
        std::vector<std::string> strategies;

        // Flags
        bool shouldSave = false;
        bool shouldLoad = false;
        bool shouldEvolve = false;

        // Evolution params (only if shouldEvolve)
        int population = 100;
        int generations = 50;
        float mutation = 0.01;

    };
}
