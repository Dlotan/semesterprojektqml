#ifndef RANDOMRANGECLASS_H
#define RANDOMRANGECLASS_H

#include <QList>

class RandomRangeClass
{
public:
    RandomRangeClass(double min, double max);
    ~RandomRangeClass();
    double getMin() const;
    void setMin(double min);
    double getMax() const;
    void setMax(double max);
    void setDeleted();
    bool isDeleted() const;
    void addValue(double value);
    int valuesCount() const;
    QList<double> getValues() const;
    void operator +=(RandomRangeClass& other);

private:
    double min;
    double max;
    QList<double> values;
    bool deleted = false;
};

#endif // RANDOMRANGECLASS_H
