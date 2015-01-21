#ifndef GENERATOR_H
#define GENERATOR_H

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_E
#define M_E 2.71828182845904523536
#endif


class Generator
{
public:
    Generator();
    ~Generator();

    virtual double getNumber() = 0;
    virtual double getTest(double x) = 0;

    double fRand(double fMin, double fMax);
};

#endif // GENERATOR_H
