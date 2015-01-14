#include "generatorparabolic.h"
#include <QtGlobal>
#include "math.h"

GeneratorParabolic::GeneratorParabolic()
{

}

GeneratorParabolic::~GeneratorParabolic()
{

}

double GeneratorParabolic::parabola(double x)
{
    if ( x < xMin || x > xMax )
    {
        return 0.0;
    }
    double a = 0.5 * ( xMin + xMax );
    double b = 0.5 * ( xMax - xMin );
    double yMax = 3.0 / ( 4. * b );
    return yMax * (1.0 - (x - a) * (x - a) / (b * b) );
}

double GeneratorParabolic::userSpecified(double yMin, double yMax)
{
    Q_ASSERT(xMin < xMax);
    Q_ASSERT(yMin < yMax);
    double x;
    double y;
    double areaMax = (xMax - xMin) * (yMax - yMin);
    do
    {
        x = (0.0 + (areaMax) * fRand(0, 1)) / (yMax - yMin) + xMin;
        y = yMin + (yMax - yMin) * fRand(0, 1);
    }while(y > parabola(x));
    return x;
}

double GeneratorParabolic::getNumber()
{
    Q_ASSERT( xMin < xMax );
    double a = 0.5 * ( xMin + xMax );
    double yMax = parabola(a);
    return userSpecified(0, yMax);
}

double GeneratorParabolic::getTest(double x)
{
    double a = (xMin + xMax) / 2;
    double b = (xMax - xMin) / 2;
    return ((a + 2*b - x)) * pow(x - a + b, 2) / (4 * pow(b, 3));
}

