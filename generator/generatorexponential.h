#ifndef GENERATOREXPONENTIAL_H
#define GENERATOREXPONENTIAL_H
#include "generator.h"

class GeneratorExponential : public Generator
{
public:
    GeneratorExponential();
    ~GeneratorExponential();

    virtual double getNumber() override;
    virtual double getTest(double x) override;
private:
    double a = 0;
    double b = 0.5;
};

#endif // GENERATOREXPONENTIAL_H
