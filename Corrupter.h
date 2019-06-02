#ifndef Corrupter_Included
#define Corrupter_Included

#include <cstddef>
#include <vector>
#include <random>

class Corrupter {
    public:

        Corrupter();
        Corrupter(double corruption_rate, size_t delta);

        void CorruptArray(std::vector<size_t>& array);

    private:

        size_t n_corruptions;
        double corruption_rate;
        size_t delta;
        std::uniform_real_distribution<> generator;
        std::mt19937 rng;
};

#endif