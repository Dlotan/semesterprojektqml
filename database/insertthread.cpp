#include "insertthread.h"
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QSqlError>

InsertThread::InsertThread(QSqlQuery &query, QString tableName, QList<int> &numbers, QObject* parent)
    : QThread(parent)
{
    this->query = query;
    this->tableName = tableName;
    this->numbers = numbers;
}

void InsertThread::run()
{
    int i = 1;
    for(auto number : numbers)
    {
        if(i % (numbers.size() / 100) == 0)
        {
            emit onProgress((i * 1.0) / numbers.size() * 100);
        }
        query.prepare(QString("INSERT INTO ") + tableName + " values (:number)");
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
        + QString::number(count) + " WHERE table_name = :table_name";
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

