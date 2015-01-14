#include "insertthread.h"
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QSqlError>
#include "dicemaster.h"

InsertThread::InsertThread(QSqlQuery &query, QString tableName, QList<double> &numbers, QObject* parent)
    : QThread(parent),
      query(query),
      numbers(numbers)
{
    this->tableName = tableName;
}

void InsertThread::run()
{
    int i = 1;
    double min = *std::min_element(numbers.begin(), numbers.end());
    double max = *std::max_element(numbers.begin(), numbers.end());
    QList<int> scaledNumbers = DiceMaster::scaleListUp(numbers);
    for(auto number : scaledNumbers)
    {
        if(i % (scaledNumbers.size() / 100) == 0)
        {
            emit onProgress((i * 1.0) / scaledNumbers.size() * 100);
        }
        query.prepare(QString("INSERT INTO ") + tableName + " (value) values (:number)");
        query.bindValue(":number", number);
        if(!query.exec())
        {
            qDebug() << query.lastError().text();
        }
        i++;
    }
    if(!query.exec("SELECT COUNT(*) FROM " + tableName))
    {
        qDebug() << query.lastError().text();
    }
    int count = 0;
    while(query.next())
    {
        count = query.value(0).toInt();
    }
    QString queryText = QString("UPDATE dist_tables SET own_count = ")
        + QString::number(count) + ", min = " + QString::number(min) + ", max = "
        + QString::number(max) + " WHERE table_name = :table_name";
    query.prepare(queryText);
    query.bindValue(":table_name", tableName);
    if(!query.exec())
    {
        qDebug() << query.lastError().text();
    }
}

InsertThread::~InsertThread()
{

}

