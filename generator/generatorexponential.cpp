#include "generatorexponential.h"
#include "math.h"
#include <QtGlobal>

GeneratorExponential::GeneratorExponential()
{

}

GeneratorExponential::~GeneratorExponential()
{

}

double GeneratorExponential::getNumber()
{
    Q_ASSERT(b > 0);
    return a - b * log(fRand(0, 1));
}

double GeneratorExponential::getTest(double x)
{
    return 1 - exp((-1*(x - a)) / b);
}

