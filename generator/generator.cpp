#include "generator.h"
#include <random>

Generator::Generator()
{

}

Generator::~Generator()
{

}

double Generator::fRand(double fMin, double fMax)
{
    std::default_random_engine rng(std::random_device{}());
    std::uniform_real_distribution<double> dist(fMin, fMax);
    return dist(rng);
}

