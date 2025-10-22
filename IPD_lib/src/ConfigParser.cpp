#include "../include/ConfigParser.h"

#include <charconv>
#include <filesystem>
#include <stdexcept>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace Engine::ConfigParser {

    namespace Args {

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
            {"--mutation", ArgTypes::MUTATION},
            {"--scb", ArgTypes::SCB}
        };

    }

    namespace Strategies {

        inline const std::unordered_map<std::string_view, StrategyTypes> strategyMap = {
            {"ALLC", StrategyTypes::ALLC},
            {"ALLD", StrategyTypes::ALLD},
            {"TFT", StrategyTypes::TFT},
            {"GRIM", StrategyTypes::GRIM},
            {"PAVLOV", StrategyTypes::PAVLOV},
            {"RND0.3", StrategyTypes::RND03},
            {"CONTRITE", StrategyTypes::CONTRITE},
            {"PROBER", StrategyTypes::PROBER}
        };

    }

    ArgTypes validateArg(std::string_view arg) {
        auto it = Args::argMap.find(arg);

        if (it == Args::argMap.end())
            throw std::invalid_argument("Invalid argument: " + std::string(arg));

        return it->second;
    }

    void validateFormat(std::string_view format) {
        if (format != "text" && format != "csv" && format != "json")
            throw std::invalid_argument("Invalid parameter for --format argument: " + std::string(format) + "\nUse --format {text | csv | json}");
    }

    std::array<double, 4> validatePayoffs(const std::string_view &param) {
        std::array<double, 4> payoffs{};
        auto first = param.data(); auto last = param.data() + param.size();
        double value{};
        auto res = std::from_chars(first, last, value);
        int i = 0;


        while (res.ptr != last) {
            if (res.ec == std::errc::invalid_argument || res.ptr[0] != ',' || value <= 0)
                throw std::invalid_argument("Invalid parameter for --payoffs argument: " + std::string(param) + "\nUse --payoffs T,R,P,S where T,R,P,S are positive values, integral or floating-point types, each seperated by commas");
            if (res.ec == std::errc::result_out_of_range)
                throw std::invalid_argument("A given value for --payoffs argument is larger than a float: " + std::string(param) + "\nUse --payoffs T,R,P,S where T,R,P,S are integral or floating-point types, each seperated by commas");

            payoffs[i++] = value;
            res = std::from_chars(res.ptr + 1, last, value);
        }
        payoffs[i] = value; // Add last element

        if (i < 3)
            throw std::invalid_argument("Invalid number of parameters for --payoffs argument: " + std::string(param) + "\nUse --payoffs T,R,P,S where T,R,P,S are positive values, integral or floating-point types, each seperated by commas");
        if (!(payoffs[0] > payoffs[1] && payoffs[1] > payoffs[2] && payoffs[2] > payoffs[3]))
            throw std::invalid_argument("Invalid parameter for --payoffs argument: " + std::string(param) + "\nUse --payoffs T,R,P,S where T > R > P > S");
        if (!(2 * payoffs[1] > payoffs[0] + payoffs[3]))
            throw std::invalid_argument("Invalid parameter for --payoffs argument: " + std::string(param) + "\nUse --payoffs T,R,P,S where 2R > T + S");

        return payoffs;
    }

    StrategyTypes validateStrategies(const std::string_view& strategy){
        auto it = Strategies::strategyMap.find(strategy);

        if (it == Strategies::strategyMap.end())
            throw std::invalid_argument("Invalid strategy for --strategies argument: " + std::string(strategy) + "\nValid strategies: ALLC,ALLD,TFT,GRIM,PAVLOV,RND0.3,CONTRITE,PROBER");

        return it->second;
    }

    std::vector<StrategyTypes> parseStrategies(const std::string_view& param) {
        std::vector<StrategyTypes> strategies;
        strategies.reserve(10); // At most 10 strategies
        size_t i = 0;
        size_t pos = param.find(',', i);
        std::string stratList = param.data();

        bool ignoreDuplicatesMessage = false;
        while (pos < std::string_view::npos) {
            std::string_view strategy = param.substr(i, pos - i);
            StrategyTypes stratType = validateStrategies(strategy);
            if (std::ranges::find(strategies, stratType) != strategies.end()) {
                ignoreDuplicatesMessage = true;
                stratList.erase(i, pos-i+1);
            }
            else
                strategies.emplace_back(stratType);

            i = pos + 1;
            pos = param.find(',', i);
        }
        std::string_view strategy = param.substr(i, pos - i);
        StrategyTypes stratType = validateStrategies(strategy);
        if (std::ranges::find(strategies, stratType) != strategies.end()) {
            ignoreDuplicatesMessage = true;
            stratList.erase(i, pos-i+1);
        }
        else
            strategies.emplace_back(stratType);



        if (ignoreDuplicatesMessage) {
            std::cout << "Duplicate strategies have been omitted. Final strategies list: " << stratList << std::endl;
        }

        return strategies;
    }

    void validateFileName(const std::string_view& fileName, bool shouldLoad){
        size_t dot = fileName.find('.');
        if (dot == std::string_view::npos)
            throw std::invalid_argument("Invalid file name: " + std::string(fileName) + "\nFile name must contain a text file extension (.txt)");
        if (std::strcmp(&fileName.at(dot + 1), "txt") != 0)
            throw std::invalid_argument("Invalid file name: " + std::string(fileName) + "\nFile name must contain a text file extension (.txt)");

        if (shouldLoad && !std::filesystem::exists(fileName))
            throw std::invalid_argument("File does not exist: " + std::string(fileName));
    }

}
