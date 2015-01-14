#include "generatorextremevalue.h"

#include "math.h"
#include <QtGlobal>

GeneratorExtremeValue::GeneratorExtremeValue()
{

}

GeneratorExtremeValue::~GeneratorExtremeValue()
{

}

double GeneratorExtremeValue::getNumber()
{
    Q_ASSERT(b > 0);
    return a + b * log(-1 * log(fRand(0, 1)));
}

double GeneratorExtremeValue::getTest(double x)
{
    return 1 - exp(-1 * exp((x - a) / b));
}

