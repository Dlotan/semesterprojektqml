#include "generatorcauchy.h"
#include <math.h>
#include <QtGlobal>

GeneratorCauchy::GeneratorCauchy()
{

}

GeneratorCauchy::~GeneratorCauchy()
{

}


double GeneratorCauchy::getNumber()
{
    double randomNumber = fRand(-0.5, 0.5);
    Q_ASSERT(b > 0);
    return a + b * tan(M_PI * randomNumber);
}

double GeneratorCauchy::getTest(double x)
{
    return 0.5 + ((1 / M_PI) * atan((x - a) / b));
}
