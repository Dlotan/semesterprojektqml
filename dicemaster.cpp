#include "dicemaster.h"
#include <QPointer>
#include <QList>
#include <QDebug>
#include <QtGlobal>
#include <QVector>
#include "generator/generator.h"
#include "generator/generatorcosine.h"
#include "generator/generatorcauchy.h"
#include "generator/generatorweibull.h"
#include "generator/generatornormal.h"
#include "generator/generatorlognormal.h"
#include "generator/generatorparabolic.h"
#include "generator/generatorextremevalue.h"
#include "generator/generatorexponential.h"
#include "generator/generatordoublelog.h"
#include <math.h>

DiceMaster::DiceMaster()
{

}

DiceMaster::~DiceMaster()
{

}

QList<int> DiceMaster::scaleListUp(const QList<double>& numbers)
{
    QList<int> result;
    double max = *std::max_element(numbers.begin(), numbers.end());
    double min = *std::min_element(numbers.begin(), numbers.end());
    max -= min;
    double multiplier = 100000000 / max;
    for(auto number : numbers)
    {
        result << (number - min) * multiplier;
    }
    return result;
}

// http://stackoverflow.com/questions/5294955/how-to-scale-down-a-range-of-numbers-with-a-known-min-and-max-value
QList<double> DiceMaster::scaleListDownDouble(const QList<double> &numbers, double min, double max)
{
    double maxElement = *std::max_element(numbers.begin(), numbers.end());
    double minElement = *std::min_element(numbers.begin(), numbers.end());
    QList<double> result;
    auto scaleFunc = [=](double x){return ((max - min) * (x - minElement)) / (maxElement - minElement) + min;};
    for(auto number : numbers)
    {
        result << scaleFunc(number);
    }
    return result;
}

QList<double> DiceMaster::scaleListDownInt(const QList<int> &numbers, double min, double max)
{
    QList<double> doubleNumbers;
    for(auto number : numbers)
    {
        doubleNumbers << number;
    }
    return scaleListDownDouble(doubleNumbers, min, max);
}

bool DiceMaster::checkChiSquare(std::unique_ptr<Generator>& generator, QList<RandomRangeClass>& randomRangeClasses)
{
    int numbersTotal = 0;
    for(const auto& randomRangeClass : randomRangeClasses)
    {
        numbersTotal += randomRangeClass.valuesCount();
    }
    if (randomRangeClasses.size() <= 100) {
        qDebug() << "Not enough Classes initial";
        return false;
    }
    double chi_square = 0;
    for (const auto& randomRangeClass : randomRangeClasses)
    {
        double erwartet = generator->getTest(randomRangeClass.getMax())
                - generator->getTest(randomRangeClass.getMin());
        erwartet *= numbersTotal;
        chi_square += pow(randomRangeClass.valuesCount() - erwartet, 2) / erwartet;
    }
    int j = randomRangeClasses.size();
    double z = 1.282;
    double checksumme = sqrt(2 * (j - 1));
    checksumme = pow(checksumme + z, 2);
    checksumme *= 0.5;
    qDebug() << "checksumme" << checksumme << "chi_square" << chi_square;
    return checksumme > chi_square;
}

QList<RandomRangeClass> DiceMaster::getRandomRangeClasses(const QList<double>& numbers, int initialClasses)
{
    QList<RandomRangeClass> result;
    double max = *std::max_element(numbers.begin(), numbers.end());
    double min = *std::min_element(numbers.begin(), numbers.end());
    qDebug() << "min" << min;
    qDebug() << "max" << max;
    double range = (max - min) / initialClasses;
    double currentMin = min;
    QVector<double> copyNumbers(numbers.size());
    std::copy(numbers.begin(), numbers.end(), copyNumbers.begin());
    std::sort(copyNumbers.begin(), copyNumbers.end());
    QList<RandomRangeClass> randomRangeClasses;
    int currentIndex = 0;
    while (currentMin < max)
    {
        double currentMax = currentMin + range;
        RandomRangeClass temp(currentMin, currentMax);
        while (currentIndex < numbers.size()
                && copyNumbers.at(currentIndex) < currentMax)
        {
            temp.addValue(copyNumbers.at(currentIndex));
            currentIndex++;
        }
        randomRangeClasses << std::move(temp);
        currentMin += range;
    }
    for (int i = 0; i < randomRangeClasses.size(); ++i)
    {
        if (randomRangeClasses.at(i).valuesCount() < 5)
        {
            int j = i + 1;
            for (; j < randomRangeClasses.size(); ++j)
            {
                if (randomRangeClasses.at(j).valuesCount() == 0)
                {
                    randomRangeClasses[j].setDeleted();
                }
                else
                {
                    randomRangeClasses[j] += randomRangeClasses[i];
                    break;
                }
            }
            i = j - 1; // j wird direkt inkrementiert desswegen -1
        }
    }
    for (auto& randomRangeClass : randomRangeClasses)
    {
        if (!randomRangeClass.isDeleted())
        {
            result << std::move(randomRangeClass);
        }
    }
    return result;
}

QList<double> DiceMaster::getTestedList(std::unique_ptr<Generator>& generator, int quantity, int initialClasses)
{
    QList<double> result;
    QList<RandomRangeClass> randomRangeClasses;
    const int maxIterations = 100;
    int i = 0;
    do {
        result.clear();
        for(int i = 0; i < quantity; i++)
        {
            result << generator->getNumber();
        }
        randomRangeClasses = getRandomRangeClasses(result, initialClasses);
        i++;
    }while(!checkChiSquare(generator, randomRangeClasses) && i < maxIterations);
    if(i == maxIterations)
    {
        return QList<double>();
    }
    return result;
}

std::unique_ptr<Generator> DiceMaster::getGeneratorFromName(QString generatorName)
{
    std::unique_ptr<Generator> generator;
    if(generatorName == "cosine")
    {
        generator = std::unique_ptr<Generator>(static_cast<Generator*>(new GeneratorCosine()));
    }
    else if(generatorName == "cauchy")
    {
        generator = std::unique_ptr<Generator>(static_cast<Generator*>(new GeneratorCauchy()));
    }
    else if(generatorName == "weibull")
    {
        generator = std::unique_ptr<Generator>(static_cast<Generator*>(new GeneratorWeibull()));
    }
    else if(generatorName == "normal")
    {
        generator = std::unique_ptr<Generator>(static_cast<Generator*>(new GeneratorNormal()));
    }
    else if(generatorName == "logNormal")
    {
        generator = std::unique_ptr<Generator>(static_cast<Generator*>(new GeneratorLognormal()));
    }
    else if(generatorName == "parabolic")
    {
        generator = std::unique_ptr<Generator>(static_cast<Generator*>(new GeneratorParabolic()));
    }
    else if(generatorName == "extremeValue")
    {
        generator = std::unique_ptr<Generator>(static_cast<Generator*>(new GeneratorExtremeValue()));
    }
    else if(generatorName == "exponential")
    {
        generator = std::unique_ptr<Generator>(static_cast<Generator*>(new GeneratorExponential()));
    }
    else if(generatorName == "doubleLog")
    {
        generator = std::unique_ptr<Generator>(static_cast<Generator*>(new GeneratorDoubleLog()));
    }
    else
    {
        Q_ASSERT(false);
    }
    return generator;
}

QList<double> DiceMaster::getRandomNumbers(QString generatorName, int quantity, int initialClasses)
{
    std::unique_ptr<Generator> generator = getGeneratorFromName(generatorName);
    QList<double> resultDoubleList = getTestedList(generator, quantity, initialClasses);
    if(resultDoubleList.size() == 0)
    {
        return QList<double>();
    }
    Q_ASSERT(resultDoubleList.size() == quantity);
    return resultDoubleList;
}

