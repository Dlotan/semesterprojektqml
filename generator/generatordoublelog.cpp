#include "generatordoublelog.h"
#include "math.h"
#include <QtGlobal>

GeneratorDoubleLog::GeneratorDoubleLog()
{

}

GeneratorDoubleLog::~GeneratorDoubleLog()
{

}

double GeneratorDoubleLog::getNumber()
{
    Q_ASSERT(xMin < xMax);
    double a = 0.5 * (xMin + xMax);
    double b = 0.5 * (xMax - xMin);
    if(fRand(0, 1) < 0.5)
    {
        return a + b * fRand(0, 1) * fRand(0, 1);
    }
    else
    {
        return a - b * fRand(0, 1) * fRand(0, 1);
    }
}

double GeneratorDoubleLog::getTest(double x)
{
    double a = (xMin + xMax) / 2.0;
    double b = (xMax - xMin) / 2.0;
    if(x >= xMin && x <= a)
    {
        return 0.5 - (fabs(x - a) / (2*b)) * (1 - log(fabs(x - a) / b));
    }
    else if(x >= a && x <= xMax)
    {
        return 0.5 + (fabs(x - a) / (2*b)) * (1 - log(fabs(x - a) / b));
    }
    else
    {
        Q_ASSERT(false);
    }
    return 0;
}

