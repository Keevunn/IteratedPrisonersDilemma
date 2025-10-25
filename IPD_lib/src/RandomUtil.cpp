#include "../include/RandomUtil.h"

#include "../include/GameConfig.h"

namespace Random {
    std::mt19937 Random::generator(GameConfig::GameConfig::seed);

    void Random::seed(const unsigned int seed) {
        generator.seed(seed);
    }

    double Random::getRandomDouble() {
        return distribution(generator);
    }

    double Random::getRandomDouble(const double min, const double max) {
        return min + (max - min) * getRandomDouble();
    }
}

