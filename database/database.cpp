#include "database.h"
#include <QDebug>
#include <QString>
#include <QSqlError>
#include "../dicemaster.h"
#include <algorithm>
#include <QElapsedTimer>
#include <QThread>
#include "insertthread.h"
#include "virusinsertthread.h"
#include "../generator/generator.h"
#include "../randomrangeclass.h"

/*
 * id table_name own_distribution own_count virus_distribution virus_count status
 */

Database::Database(QObject *parent) : QObject(parent)
{
    permDatabaseConnection = QSqlDatabase::addDatabase("QSQLITE");
    permDatabaseConnection.setDatabaseName("bla");
    if(!permDatabaseConnection.open())
    {
        qDebug() << "cant open db";
    }
    query = QSqlQuery(permDatabaseConnection);
    query.prepare(QString("CREATE TABLE IF NOT EXISTS dist_tables(id INTEGER PRIMARY KEY ASC AUTOINCREMENT,")
        + "table_name TEXT, own_distribution TEXT, own_count INTEGER DEFAULT 0,"
        + "virus_distribution TEXT, virus_count INTEGER DEFAULT 0, status TEXT,"
        + "has_index INTEGER, min REAL DEFAULT 0, max REAL DEFAULT 0)");
    if(!query.exec())
    {
        query.lastQuery();
        qDebug() << query.lastError().text();
    }
}

void Database::createTable(QString tableName, QString distribution, bool hasIndex)
{
    query.prepare(QString("INSERT INTO dist_tables(table_name, own_distribution, status, has_index) ")
        + "VALUES (:table_name, :distribution, :status, :has_index)");
    query.bindValue(":table_name", tableName);
    query.bindValue(":distribution", distribution);
    query.bindValue(":status", "normal");
    query.bindValue(":has_index", hasIndex ? 1 : 0);
    if(!query.exec())
    {
        qDebug() << query.lastError().text();
    }
    if(!query.exec(QString("CREATE TABLE " + tableName + " (id INTEGER PRIMARY KEY ASC AUTOINCREMENT, value INTEGER)")))
    {
        qDebug() << query.lastError().text();
    }
    if(hasIndex)
    {
        if(!query.exec("CREATE INDEX " + tableName + "index ON " + tableName + " (value)"))
        {
                qDebug() << query.lastError().text();
        }
    }
    qDebug() << "Table" << tableName << "with distribution" << distribution << "created.";
}

void Database::deleteTable(QString tableName)
{
    if(!query.exec("DROP TABLE " + tableName))
    {
        qDebug() << query.lastError().text();
    }
    if(!query.exec("DROP INDEX IF EXISTS " + tableName + "index"))
    {
        qDebug() << query.lastError().text();
    }
    query.prepare("DELETE FROM dist_tables WHERE table_name = :table_name");
    query.bindValue(":table_name", tableName);
    if(!query.exec())
    {
        qDebug() << query.lastError().text();
    }
}

void Database::resetDatabase()
{
    query.exec("SELECT table_name FROM dist_tables");
    QStringList tableNames;
    while(query.next())
    {
        tableNames << query.value(0).toString();
    }
    for(const auto& tableName : tableNames)
    {
        deleteTable(tableName);
    }
    query.exec("DROP TABLE dist_tables");
}

bool Database::fillTable(QString tableName, int quantity, int initialClasses)
{
    query.prepare("SELECT own_distribution, status, min, max, has_index FROM dist_tables WHERE table_name = :table_name");
    query.bindValue(":table_name", tableName);
    if(!query.exec())
    {
        qDebug() << query.lastError().text();
    }
    if(!query.next())
    {
        return false;
    }
    QString distribution = query.value(0).toString();
    QString status = query.value(1).toString();
    double min = query.value(2).toDouble();
    double max = query.value(3).toDouble();
    int hasIndex = query.value(4).toInt();
    if(status != "normal")
    {
        emit onError("Invalid status " + status);
        return false;
    }
    QList<double> numbers;
    // Check if already numbers in it;
    if(!(min == 0 && max == 0))
    {
        QVariantList oldNumbers = getNumbers(tableName);
        QList<int> unscaledNumbers;
        for(const auto& oldNumber : oldNumbers)
        {
            unscaledNumbers << oldNumber.toInt();
        }
        numbers << DiceMaster::scaleListDownInt(unscaledNumbers, min, max);
    }
    numbers << DiceMaster::getRandomNumbers(distribution, quantity, initialClasses);
    if(numbers.size() == 0)
    {
        emit onError("Initial classes make chi square fail");
        return false;
    }
    deleteTable(tableName);
    createTable(tableName, distribution, hasIndex);
    QSqlQuery query2(permDatabaseConnection);
    InsertThread* insertThread = new InsertThread(query2, tableName, numbers, this);
    connect(insertThread, &InsertThread::onProgress, [=](int progress){emit onProgress(progress);});
    connect(insertThread, &InsertThread::finished, [=](){emit onFillFinished();});
    connect(insertThread, &InsertThread::finished, insertThread, &QObject::deleteLater);
    insertThread->start();
    return true;
}

QVariantList Database::getNumbers(QString tableName)
{
    QVariantList result;
    if(!query.exec("SELECT value FROM " + tableName))
    {
        qDebug() << query.lastError().text();
    }
    while(query.next())
    {
        result << query.value(0).toInt();
    }
    return result;
}

int Database::profileQuery(QString queryString)
{
    const int warmup = 5;
    const int iterations = 10;
    // Warmup.
    for(int i = 0; i < warmup; ++i)
    {
        QSqlDatabase* tempConnection = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", "temp"));
        tempConnection->setDatabaseName("bla");
        if(!tempConnection->open())
        {
            qDebug() << "cant open DB";
        }
        QSqlQuery tempQuery(*tempConnection);
        if(!tempQuery.exec(queryString))
        {
            qDebug() << tempQuery.lastError().text();
        }
        tempConnection->close();
        delete tempConnection;
        QSqlDatabase::removeDatabase("temp");
    }
    QList<int> results;
    for(int i = 0; i < iterations; ++i)
    {
        QSqlDatabase* tempConnection = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", "temp"));
        tempConnection->setDatabaseName("bla");
        if(!tempConnection->open())
        {
            qDebug() << "cant open DB";
        }
        QSqlQuery tempQuery(*tempConnection);
        QElapsedTimer timer;
        timer.start();
        if(!tempQuery.exec(queryString))
        {
            qDebug() << tempQuery.lastError().text();
        }
        results << timer.nsecsElapsed();
        tempConnection->close();
        delete tempConnection;
        QSqlDatabase::removeDatabase("temp");
    }
    int sum = 0;
    for(auto result : results)
    {
        sum += result;
    }
    return sum / results.size();
}

int Database::closest(const QList<int> &numbers, int search)
{
    int oldDifference = abs(search - numbers.first());
    int oldNumber = numbers.first();
    for(auto newNumber : numbers)
    {
        int newDifference = abs(search - newNumber);
        if(newDifference > oldDifference)
        {
            return oldNumber;
        }
        oldNumber = newNumber;
        oldDifference = newDifference;
    }
    return oldNumber;
}

QVariantList Database::profileTable(QString tableName)
{
    qDebug() << "profile" << tableName;
    QVariantList result;
    QVariantList numbers = getNumbers(tableName);
    QList<int> sortedNumbers;
    for(auto number : numbers)
    {
        sortedNumbers << number.toInt();
    }
    std::sort(sortedNumbers.begin(), sortedNumbers.end());
    QList<int> searchFor;
    const int min = sortedNumbers.first();
    const int max = sortedNumbers.last();
    searchFor << min;
    searchFor << max;
    searchFor << sortedNumbers[sortedNumbers.length() / 2];
    const int midRange = (max - min) / 2;
    searchFor << *std::lower_bound(sortedNumbers.begin(), sortedNumbers.end(), closest(sortedNumbers, midRange));
    int i = min + 1;
    while(std::binary_search(sortedNumbers.begin(), sortedNumbers.end(), i))
    {
        i++;
    }
    searchFor << i;
    i = max - 1;
    while(std::binary_search(sortedNumbers.begin(), sortedNumbers.end(), i))
    {
        i--;
    }
    searchFor << i;
    i = sortedNumbers[sortedNumbers.length() / 2];
    while(std::binary_search(sortedNumbers.begin(), sortedNumbers.end(), i))
    {
        i++;
    }
    searchFor << i;
    i = midRange;
    while(std::binary_search(sortedNumbers.begin(), sortedNumbers.end(), i))
    {
        i++;
    }
    searchFor << i;
    for(auto number : searchFor)
    {
        result << profileQuery("SELECT * FROM " + tableName + " WHERE value = " + QString::number(number));
    }
    return result;
}

QVariantMap Database::profile()
{
    QVariantMap result;
    if(!query.exec("SELECT table_name, own_count, virus_count FROM dist_tables"))
    {
        qDebug() << query.lastError().text();
    }
    QStringList tables;
    while(query.next())
    {
        if(query.value(1).toInt() == 0 && query.value(2).toInt() == 0)
        {
            continue;
        }
        tables << query.value(0).toString();
    }
    for(auto table : tables)
    {
        result[table] = profileTable(table);
    }
    return result;
}

bool Database::virusInsert(QString tableName, QString virusDistribution, int quantity, int initialClasses)
{
    query.prepare("SELECT own_distribution, status, min, max, has_index FROM dist_tables WHERE table_name = :table_name");
    query.bindValue(":table_name", tableName);
    if(!query.exec())
    {
        qDebug() << query.lastError().text();
    }
    if(!query.next())
    {
        return false;
    }
    QString ownDistribution = query.value(0).toString();
    QString status = query.value(1).toString();
    double min = query.value(2).toDouble();
    double max = query.value(3).toDouble();
    int hasIndex = query.value(4).toInt();
    if(status != "normal")
    {
        emit onError("Invalid status " + status);
        return false;
    }
    QVariantList oldNumbersVariant = getNumbers(tableName);
    QList<int> oldNumbersInt;
    for(const auto& oldNumberVariant : oldNumbersVariant)
    {
        oldNumbersInt << oldNumberVariant.toInt();
    }
    const QList<double> oldNumbersScaled = DiceMaster::scaleListDownInt(oldNumbersInt, min, max);
    QList<double> oldNumbers = oldNumbersScaled;
    QList<double> newNumbers;
    std::unique_ptr<Generator> ownGenerator = DiceMaster::getGeneratorFromName(ownDistribution);
    int i = 0;
    QList<RandomRangeClass> randomRangeClasses =
        DiceMaster::getRandomRangeClasses(oldNumbers + newNumbers, initialClasses * (oldNumbers.size() / quantity));
    while(DiceMaster::checkChiSquare(ownGenerator, randomRangeClasses))
    {
        newNumbers += DiceMaster::scaleListDownDouble(DiceMaster::getRandomNumbers(virusDistribution, quantity, initialClasses), min, max);
        int oldNumbersOriginalSize = oldNumbers.size();
        for(auto j = 0; j < oldNumbers.size(); j += (oldNumbersOriginalSize / quantity) - 1)
        {
            oldNumbers.removeAt(j);
        }
        randomRangeClasses =
            DiceMaster::getRandomRangeClasses(oldNumbers + newNumbers, initialClasses * (oldNumbers.size() / quantity));
        i++;
    }
    if(i == 0)
    {
        emit onError("Mit klassenmenge Chi Square schon am anfang nicht erfüllt");
        return false;
    }
    QString virusTableName = tableName + "_virus";
    createTable(virusTableName, ownDistribution, hasIndex ? true : false);
    query.prepare("UPDATE dist_tables SET virus_distribution = :virus_distribution WHERE table_name = :table_name");
    query.bindValue(":table_name", virusTableName);
    query.bindValue(":virus_distribution", virusDistribution);
    if(!query.exec())
    {
        qDebug() << query.lastError().text();
    }
    QList<double> oldNumbersScaledCopy = oldNumbersScaled;
    VirusInsertThread* virusInsertThread = new VirusInsertThread(query, virusTableName, oldNumbersScaledCopy, newNumbers, this);
    connect(virusInsertThread, &VirusInsertThread::onProgress, [=](int progress){emit onProgress(progress);});
    connect(virusInsertThread, &VirusInsertThread::finished, [=](){emit onFillFinished();});
    connect(virusInsertThread, &VirusInsertThread::finished, virusInsertThread, &QObject::deleteLater);
    virusInsertThread->start();
    return true;
}

Database::~Database()
{

}

