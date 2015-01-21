#ifndef DICEMASTER_H
#define DICEMASTER_H

#include <QList>
#include <memory>
#include "randomrangeclass.h"
#include "generator/generator.h"

class DiceMaster
{
public:
    DiceMaster();
    ~DiceMaster();

    static QList<int> scaleListUp(const QList<double> &numbers);
    static QList<double> scaleListDownDouble(const QList<double>& numbers, double min, double max);
    static QList<double> scaleListDownInt(const QList<int>& numbers, double min, double max);
    static bool checkChiSquare(std::unique_ptr<Generator>& generator, QList<RandomRangeClass> &randomRangeClasses);
    static QList<RandomRangeClass> getRandomRangeClasses(const QList<double> &numbers, int initialClasses);
    static QList<double> getTestedList(std::unique_ptr<Generator>& generator, int quantity, int initialClasses);
    static std::unique_ptr<Generator> getGeneratorFromName(QString generatorName);
    static QList<double> getRandomNumbers(QString generatorName, int quantity, int initialClasses);

    static double getAverage(QList<int> numbers);
    static double getStdDeviation(QList<int> numbers);
};

#endif // DICEMASTER_H
