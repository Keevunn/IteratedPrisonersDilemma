#include "Engine.h"

#include <complex>
#include <cstring>
#include <stdexcept>
#include <unordered_map>

namespace Engine {

    void Engine::parseArgs(int argc, char* argv[]) {
        std::vector<std::string_view> args(argv, argv + argc);

        for (int i = 1; i < argc; i++) { // flags in every odd position: 1, 3, 5 etc...
            ArgTypes arg = validateArg(args[i]);

            switch (arg) {
                case ArgTypes::ROUNDS:
                    rounds = parseParam<int>(args[++i], "--rounds"); break;
                case ArgTypes::REPEATS:
                    repeats = parseParam<int>(args[++i], "--repeats"); break;
                case ArgTypes::SEED:
                    seed = parseParam<int>(args[++i], "--seed"); break;
                case ArgTypes::EPSILON:
                    epsilon = parseParam<float>(args[++i], "--epsilon"); break;
                case ArgTypes::PAYOFFS:
                    // TODO validatePayoffs()
                    break;
                case ArgTypes::STRATEGIES:
                    // TODO validateStrategies()
                    break;
                case ArgTypes::FORMAT:
                    validateFormat(args[++i]);
                    format = args[i]; break;
                case ArgTypes::SAVE:
                    shouldSave = true;
                    fileName = args[++i]; break; // TODO validate by trying to open the file
                case ArgTypes::LOAD:
                    shouldLoad = true;
                    fileName = args[++i]; break;
                case ArgTypes::EVOLVE:
                    shouldEvolve = true;
                    if (std::strcmp(&args[i+1].at(0), "1") == 0) i += 1;
                    else if (std::strcmp(&args[i+1].at(0), "-") != 0)
                        throw std::invalid_argument("Invalid parameter for --evolve argument: " + std::string(args[i+1]) + "\nUse --evolve 1 or --evolve to enable evolution");
                    break;
                case ArgTypes::POPULATION:
                    population = parseParam<int>(args[++i], "--population"); break;
                case ArgTypes::GENERATIONS:
                    generations = parseParam<int>(args[++i], "--generations"); break;
                case ArgTypes::MUTATION:
                    mutation = parseParam<float>(args[++i], "--mutation"); break;

            }
        }
    }

    Engine::ArgTypes Engine::validateArg(std::string_view arg) {
        auto it = argMap.find(arg);

        if (it == argMap.end())
            throw std::invalid_argument("Invalid argument: " + std::string(arg));

        return it->second;
    }

    void Engine::validateFormat(std::string_view format) {
        if (format != "text" && format != "csv" && format != "json")
            throw std::invalid_argument("Invalid parameter for --format argument: " + std::string(format) + "\nUse --format {text | csv | json}");
    }

    void Engine::validatePayoffs(const std::string_view &param) const {
        auto first = param.begin(); auto last = param.end();
        float value{};
        auto res = std::from_chars(first, last, value);

    }



};
