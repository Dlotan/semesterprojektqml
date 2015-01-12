#ifndef GENERATORCAUCHY_H
#define GENERATORCAUCHY_H
#include "generator.h"

class GeneratorCauchy : public Generator
{
public:
    GeneratorCauchy();
    ~GeneratorCauchy();
    virtual double getNumber() override;
    virtual double getTest(double x) override;
private:
    double a = 0;
    double b = 0.5;
};

#endif // GENERATORCAUCHY_H
