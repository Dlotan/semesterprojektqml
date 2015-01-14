#ifndef GENERATOREXTREMEVALUE_H
#define GENERATOREXTREMEVALUE_H
#include "generator.h"

class GeneratorExtremeValue : public Generator
{
public:
    GeneratorExtremeValue();
    ~GeneratorExtremeValue();
    virtual double getNumber() override;
    virtual double getTest(double x) override;
private:
    double a = 0;
    double b = 0.5;
};

#endif // GENERATOREXTREMEVALUE_H
