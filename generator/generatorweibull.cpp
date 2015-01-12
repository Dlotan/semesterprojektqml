#include "generatorweibull.h"
#include "math.h"
#include <QtGlobal>

GeneratorWeibull::GeneratorWeibull()
{

}

GeneratorWeibull::~GeneratorWeibull()
{

}

double GeneratorWeibull::getNumber()
{
    Q_ASSERT (b > 0. && c > 0.);
    double randomNumber = fRand(0, 1);
    return a + b * pow(-log(randomNumber), 1 / c);
}

double GeneratorWeibull::getTest(double x)
{
    return 1 - pow(M_E, (-1 * pow(((x - a) / b), c)));
}

