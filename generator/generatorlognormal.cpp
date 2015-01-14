#include "generatorlognormal.h"
#include "math.h"

GeneratorLognormal::GeneratorLognormal()
{

}

GeneratorLognormal::~GeneratorLognormal()
{

}

double GeneratorLognormal::getNumber()
{
    return a + exp(generatorNormal.getNumber());
}

double GeneratorLognormal::getTest(double x)
{
    return generatorNormal.getTest(log(x));
}

