#pragma once
#include "../include/Engine.h"

#include <charconv>
#include <stdexcept>

namespace Engine {

    class ConfigParser {
    public:
        static Args::ArgTypes validateArg(std::string_view arg);
        static void validateFormat(std::string_view format);
        static std::array<double, 4> validatePayoffs(const std::string_view& param);
        static std::vector<Strategies::StrategyTypes> parseStrategies(const std::string_view& param);
        static void validateFileName(const std::string_view& fileName, bool shouldLoad = false);

        template<typename T> requires std::is_arithmetic_v<T>
        static T parseParam(const std::string_view& param, const std::string&& arg) {
            auto first = param.data(); auto last = param.data()+param.size();
            T value{};

            if (auto res = std::from_chars(first, last, value); res.ec != std::errc() || res.ptr != last)
                throw std::invalid_argument("Invalid parameter for " + arg + " argument: " + std::string(param));
            return value;
        }

    };



}
