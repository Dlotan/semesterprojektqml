#ifndef GENERATORPARABOLIC_H
#define GENERATORPARABOLIC_H

#include "generator.h"

class GeneratorParabolic : public Generator
{
public:
    GeneratorParabolic();
    ~GeneratorParabolic();
    double parabola(double x);
    double userSpecified(double yMin, double yMax);
    virtual double getNumber() override;
    virtual double getTest(double x) override;
private:
    double xMin = 0;
    double xMax = 1;
};

#endif // GENERATORPARABOLIC_H
