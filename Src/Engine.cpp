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
                    validatePayoffs(args[++i]); break;
                case ArgTypes::STRATEGIES:
                    parseStrategies(args[++i]);
                    break;
                case ArgTypes::FORMAT:
                    validateFormat(args[++i]);
                    format = args[i]; break;
                case ArgTypes::SAVE:
                    shouldSave = true;
                    validateFileName(args[i+1]);
                    fileName = args[++i]; break;
                case ArgTypes::LOAD:
                    shouldLoad = true;
                    validateFileName(args[i+1]);
                    fileName = args[++i]; break; // TODO validate by trying to open the file
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

    void Engine::validatePayoffs(const std::string_view &param) {
        auto first = param.data(); auto last = param.data() + param.size();
        float value{};
        auto res = std::from_chars(first, last, value);
        int i = 0;

        while (res.ptr != last) {
            if (res.ec == std::errc::invalid_argument || std::strcmp(&res.ptr[0], ",") !=0 || value <= 0)
                throw std::invalid_argument("Invalid parameter for --payoffs argument: " + std::string(param) + "\nUse --payoffs T,R,P,S where T,R,P,S are positive values, integral or floating-point types, each seperated by commas");
            if (res.ec == std::errc::result_out_of_range)
                throw std::invalid_argument("A given value for --payoffs argument is larger than a float: " + std::string(param) + "\nUse --payoffs T,R,P,S where T,R,P,S are integral or floating-point types, each seperated by commas");

            payoffs[i++] = value;
            res = std::from_chars(res.ptr + 1, last, value);
        }

        if (!(payoffs[0] > payoffs[1] && payoffs[1] > payoffs[2] && payoffs[2] > payoffs[3]))
            throw std::invalid_argument("Invalid parameter for --payoffs argument: " + std::string(param) + "\nUse --payoffs T,R,P,S where T > R > P > S");
        if (2 * payoffs[1] > payoffs[0] + payoffs[3])
            throw std::invalid_argument("Invalid parameter for --payoffs argument: " + std::string(param) + "\nUse --payoffs T,R,P,S where 2R > T + S");
    }

    Engine::StrategyTypes Engine::validateStrategies(const std::string_view& strategy){
        auto it = strategyMap.find(strategy);

        if (it == strategyMap.end())
            throw std::invalid_argument("Invalid strategy for --strategies argument: " + std::string(strategy) + "\nValid strategies: ALLC,ALLD,TFT,GRIM,PAVLOV,RND0.3,CONTRITE,PROBER");

        return it->second;
    }

    void Engine::parseStrategies(const std::string_view& param) {
        strategies.reserve(10); // At most 10 strategies
        size_t i = 0;
        size_t pos = param.find(',', i);

        while (pos != std::string_view::npos) {
            std::string_view strategy = param.substr(i, pos - i);
            strategies.emplace_back(validateStrategies(strategy));
            i = pos + 1;
            pos = param.find(',', i);
        }
    }

    void validateFileName(const std::string_view& fileName) {
        size_t dot = fileName.find('.');
        if (dot == std::string_view::npos)
            throw std::invalid_argument("Invalid file name: " + std::string(fileName) + "\nFile name must contain a text file extension (.txt)");
        if (std::strcmp(&fileName.at(dot + 1), "txt") != 0)
            throw std::invalid_argument("Invalid file name: " + std::string(fileName) + "\nFile name must contain a text file extension (.txt)");
    }

};
