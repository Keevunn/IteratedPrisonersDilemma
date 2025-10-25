#pragma once
#include <random>

namespace Random {
    class Random {
    public:
        Random() = default;

        // Should be called when initialising engine (?)
        static void seed(unsigned int seed);

        static double getRandomDouble();
        static double getRandomDouble(double min, double max);

        static std::mt19937 generator;

    private:
        inline static std::uniform_real_distribution<double> distribution{0, 1};
    };
}
