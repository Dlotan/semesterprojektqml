#ifndef GENERATOR_H
#define GENERATOR_H


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
