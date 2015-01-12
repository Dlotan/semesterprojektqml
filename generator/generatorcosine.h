#ifndef GENERATORCOSINE_H
#define GENERATORCOSINE_H
#include "generator.h"

class GeneratorCosine : public Generator
{
public:
    GeneratorCosine();
    ~GeneratorCosine();

    virtual double getNumber() override;
    virtual double getTest(double x) override;
private:
    double xMin = 0;
    double xMax = 1;
};

#endif // GENERATORCOSINE_H
