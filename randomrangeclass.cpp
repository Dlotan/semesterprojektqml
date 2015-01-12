#include "randomrangeclass.h"

RandomRangeClass::RandomRangeClass(double min, double max)
{
    this->min = min;
    this->max = max;
}

RandomRangeClass::~RandomRangeClass()
{

}

double RandomRangeClass::getMin() const
{
    return min;
}

void RandomRangeClass::setMin(double min)
{
    this->min = min;
}

double RandomRangeClass::getMax() const
{
    return max;
}

void RandomRangeClass::setMax(double max)
{
    this->max = max;
}

void RandomRangeClass::setDeleted()
{
    deleted = true;
}

bool RandomRangeClass::isDeleted() const
{
    return deleted;
}

void RandomRangeClass::addValue(double value)
{
    values << value;
}

int RandomRangeClass::valuesCount() const
{
    return values.count();
}

QList<double> RandomRangeClass::getValues() const
{
    return values;
}

void RandomRangeClass::operator +=(RandomRangeClass& other)
{
    this->min = qMin(this->getMin(), other.getMin());
    this->max = qMax(this->getMax(), other.getMax());
    values.append(other.getValues());
    other.setDeleted();
}

