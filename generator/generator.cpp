#include "generator.h"
#include <cstdlib>

Generator::Generator()
{

}

Generator::~Generator()
{

}

double Generator::fRand(double fMin, double fMax)
{
    double f = (double)std::rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

