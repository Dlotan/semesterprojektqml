#ifndef GENERATORWEIBULL_H
#define GENERATORWEIBULL_H

#include "generator.h"

class GeneratorWeibull : public Generator
{
public:
    GeneratorWeibull();
    ~GeneratorWeibull();

    virtual double getNumber() override;
    virtual double getTest(double x) override;
private:
    double a = 0;
    double b = 1;
    double c = 2;
};

#endif // GENERATORWEIBULL_H
