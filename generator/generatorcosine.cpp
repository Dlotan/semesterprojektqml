#include "generatorcosine.h"
#include "math.h"
#include <QtGlobal>

#define _USE_MATH_DEFINES

GeneratorCosine::GeneratorCosine()
{
}

GeneratorCosine::~GeneratorCosine()
{

}

double GeneratorCosine::getNumber()
{
    Q_ASSERT(xMin < xMax);
    double a = 0.5 * (xMin + xMax);
    double b = (xMax - xMin) / M_PI;
    double random_number = fRand(-1, 1);
    return a + b * asin(random_number);
}

double GeneratorCosine::getTest(double x)
{
    double a = (xMin + xMax) / 2;
    double b = (xMax - xMin) / M_PI;
    return 0.5 * (1 + sin((x - a) / b));
}

