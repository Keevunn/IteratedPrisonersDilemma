#pragma once

#include <charconv>
#include <stdexcept>
#include <vector>

#include "GameConfig.h"

using namespace GameConfig::Args;
using namespace GameConfig::Strategies;

namespace Engine::ConfigParser {

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

}

