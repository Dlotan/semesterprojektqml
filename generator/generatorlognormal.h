#ifndef GENERATORLOGNORMAL_H
#define GENERATORLOGNORMAL_H
#include "generator.h"
#include "generatornormal.h"
class GeneratorLognormal : public Generator
{
public:
    GeneratorLognormal();
    ~GeneratorLognormal();

    virtual double getNumber() override;
    virtual double getTest(double x) override;
private:
    double a = 0;
    GeneratorNormal generatorNormal;
};

#endif // GENERATORLOGNORMAL_H
