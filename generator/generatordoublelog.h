#ifndef GENERATORDOUBLELOG_H
#define GENERATORDOUBLELOG_H

#include "generator.h"

class GeneratorDoubleLog : public Generator
{
public:
    GeneratorDoubleLog();
    ~GeneratorDoubleLog();
    virtual double getNumber() override;
    virtual double getTest(double x) override;
private:
    double xMin = -1;
    double xMax = 1;
};

#endif // GENERATORDOUBLELOG_H
