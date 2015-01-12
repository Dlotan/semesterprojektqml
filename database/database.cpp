#include "database.h"
#include <QDebug>
#include <QString>
#include <QSqlError>
#include "../dicemaster.h"
#include <algorithm>
#include <QElapsedTimer>

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
        + "virus_distribution TEXT, virus_count INTEGER DEFAULT 0, status TEXT)");
    if(!query.exec())
    {
        qDebug() << query.lastError().text();
    }
}

void Database::createTable(QString tableName, QString distribution)
{
    query.prepare(QString("INSERT INTO dist_tables(table_name, own_distribution, status) ")
        + "VALUES (:table_name, :distribution, :status)");
    query.bindValue(":table_name", tableName);
    query.bindValue(":distribution", distribution);
    query.bindValue(":status", "normal");
    if(!query.exec())
    {
        qDebug() << query.lastError().text();
    }
    if(!query.exec(QString("CREATE TABLE " + tableName + " (value INTEGER)")))
    {
        qDebug() << query.lastError().text();
    }
    qDebug() << "Table" << tableName << "with distribution" << distribution << "created.";
}

void Database::deleteTable(QString tableName)
{
    if(!query.exec("DROP TABLE " + tableName))
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
    while(query.next())
    {
        QString tableName = query.value(0).toString();
        QSqlQuery query2(permDatabaseConnection);
        query2.prepare("DROP TABLE " + tableName);
        query2.exec();
    }
    query.exec("DROP TABLE dist_tables");
}

void Database::fillTable(QString tableName, int quantity, int initialClasses)
{
    query.prepare("SELECT own_distribution FROM dist_tables WHERE table_name = :table_name");
    query.bindValue(":table_name", tableName);
    if(!query.exec())
    {
        qDebug() << query.lastError().text();
    }
    while(query.next())
    {
        QString distribution = query.value(0).toString();
        QList<int> numbers = DiceMaster::getRandomNumbers(distribution, quantity, initialClasses);
        qDebug() << numbers.size();
        QSqlQuery query2(permDatabaseConnection);
        for(auto number : numbers)
        {
            query2.prepare(QString("INSERT INTO ") + tableName + " values (:number)");
            query2.bindValue(":number", number);
            if(!query2.exec())
            {
                qDebug() << query2.lastError().text();
            }
        }
    }
    if(!query.exec("SELECT COUNT(*) FROM " + tableName))
    {
        qDebug() << query.lastError().text();
    }
    while(query.next())
    {
        int count = query.value(0).toInt();
        QSqlQuery query2(permDatabaseConnection);
        QString queryText = QString("UPDATE dist_tables SET own_count = ")
            + QString::number(count) + " WHERE table_name = :table_name";
        query2.prepare(queryText);
        query2.bindValue(":table_name", tableName);
        if(!query2.exec())
        {
            qDebug() << query.lastError().text();
        }
    }
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
    qSort(sortedNumbers.begin(), sortedNumbers.end());
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

Database::~Database()
{

}

