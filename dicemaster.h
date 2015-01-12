#ifndef DICEMASTER_H
#define DICEMASTER_H

#include <QList>
#include "randomrangeclass.h"
#include "generator/generator.h"

class DiceMaster
{
public:
    DiceMaster();
    ~DiceMaster();

    static double collectionMin(const QList<double> &numbers);
    static double collectionMax(const QList<double> &numbers);
    static QList<int> transformList(const QList<double> &numbers);
    static bool checkChiSquare(std::unique_ptr<Generator>& generator, QList<RandomRangeClass> &randomRangeClasses);
    static QList<RandomRangeClass> getRandomRangeClasses(const QList<double> &numbers, int initialClasses);
    static QList<double> getTestedList(std::unique_ptr<Generator>& generator, int quantity, int initialClasses);
    static QList<int> getRandomNumbers(QString generatorName, int quantity, int initialClasses);
};

#endif // DICEMASTER_H
