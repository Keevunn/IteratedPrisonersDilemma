#pragma once

#include <charconv>
#include <stdexcept>
#include <string>
#include <vector>

#include "GameConfig.h"

using namespace GameConfig::Strategies;

namespace Engine::ConfigParser {

    enum class ArgTypes{
        ROUNDS, REPEATS, SEED, EPSILON, PAYOFFS,
        STRATEGIES, FORMAT, SAVE, LOAD,
        EVOLVE, POPULATION, GENERATIONS, MUTATION, SCB
    };

    ArgTypes validateArg(std::string_view arg);
    void validateFormat(std::string_view format);
    std::array<double, 4> validatePayoffs(const std::string_view& param);
    std::vector<StrategyTypes> parseStrategies(const std::string_view& param);
    void validateFileName(const std::string_view& fileName, bool shouldLoad = false);

    template<typename T> requires std::is_arithmetic_v<T>
    T parseParam(const std::string_view& param, const std::string&& arg) {
        auto first = param.data(); auto last = param.data()+param.size();
        T value{};

        if (auto res = std::from_chars(first, last, value); res.ec != std::errc() || res.ptr != last)
            throw std::invalid_argument("Invalid parameter for " + arg + " argument: " + std::string(param));
        return value;
    }

    template<typename T> requires std::is_arithmetic_v<T>
    T parseParam(const std::string_view& param, const std::string&& arg, const T& lowerBound) {
        auto first = param.data(); auto last = param.data()+param.size();
        T value{};

        if (auto res = std::from_chars(first, last, value); res.ec != std::errc() || res.ptr != last)
            throw std::invalid_argument("Invalid parameter for " + arg + " argument: " + std::string(param));
        if (value < lowerBound)
            throw std::invalid_argument("Parameter for " + arg + " argument must be greater than " + std::to_string(lowerBound) + ": " + std::string(param));
        return value;
    }

    template<typename T> requires std::is_arithmetic_v<T>
    T parseParam(const std::string_view& param, const std::string&& arg, const std::pair<T, T>& range) {
        auto first = param.data(); auto last = param.data()+param.size();
        T value{};

        if (auto res = std::from_chars(first, last, value); res.ec != std::errc() || res.ptr != last)
            throw std::invalid_argument("Invalid parameter for " + arg + " argument: " + std::string(param));
        if (range.first >= range.second)
            throw std::invalid_argument("Invalid range: [" + std::to_string(range.first) + ", " + std::to_string(range.second) + "]"); // Error on the developer's end
        if (value < range.first || value > range.second)
            throw std::invalid_argument("Parameter for " + arg + " argument must be in the range [" + std::to_string(range.first) + ", " + std::to_string(range.second) + "]: " + std::string(param));
        return value;
    }

}

