#ifndef GENERATORNORMAL_H
#define GENERATORNORMAL_H

#include "generator.h"
#include <QList>

class GeneratorNormal : public Generator
{
public:
    GeneratorNormal();
    ~GeneratorNormal();
    virtual double getNumber() override;
    virtual double getTest(double x) override;
private:
    double mu = 0;
    double sigma = 1;
    QList<double> vergleichswerte;
};

#endif // GENERATORNORMAL_H
