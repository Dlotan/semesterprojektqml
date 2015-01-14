#include "virusinsertthread.h"
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include "dicemaster.h"
#include <QDebug>

VirusInsertThread::VirusInsertThread(QSqlQuery &query, QString tableName,
    QList<double> &before, QList<double> &after, QObject* parent)
    : QThread(parent),
      tableName(tableName),
      query(query),
      before(before),
      after(after)
{
}

VirusInsertThread::~VirusInsertThread()
{
}

void VirusInsertThread::run()
{
    QList<double> allNumbersTogether = before + after;
    int i = 1;
    double min = *std::min_element(allNumbersTogether.begin(), allNumbersTogether.end());
    double max = *std::max_element(allNumbersTogether.begin(), allNumbersTogether.end());

    // First insert.
    QList<int> scaledBefore = DiceMaster::scaleListUp(before);
    for(auto number : scaledBefore)
    {
        if(i % (scaledBefore.size() / 100) == 0)
        {
            emit onProgress((i * 1.0) / scaledBefore.size() * 100);
        }
        query.prepare(QString("INSERT INTO ") + tableName + " (value) values (:number)");
        query.bindValue(":number", number);
        if(!query.exec())
        {
            qDebug() << query.lastError().text();
        }
        i++;
    }

    // Delete stage.
    int j = 1;
    for(int i = 0; i < scaledBefore.length(); i += (scaledBefore.size() / after.size()))
    {
        int number = scaledBefore[i];
        emit onProgress(j / after.size() * 100);
        query.prepare(QString("SELECT id FROM ") + tableName + " WHERE value=" + QString::number(number));
        query.bindValue(":number", number);
        if(!query.exec())
        {
            qDebug() << query.lastQuery();
            qDebug() << query.lastError().text();
        }
        if(!query.next())
        {
            Q_ASSERT(false);
        }
        int id = query.value(0).toInt();
        query.prepare("DELETE FROM " + tableName + " WHERE id=:id");
        query.bindValue(":id", id);
        if(!query.exec())
        {
            qDebug() << query.lastError().text();
        }
        j++;
    }

    // Virus insert.
    i = 1;
    QList<int> scaledAfter = DiceMaster::scaleListUp(after);
    for(auto number : scaledAfter)
    {
        if(i % (scaledAfter.size() / 100) == 0)
        {
            emit onProgress((i * 1.0) / scaledAfter.size() * 100);
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
        + QString::number(count - after.size()) + ", virus_count = "
        + QString::number(after.size()) + ", min = " + QString::number(min) + ", max = "
        + QString::number(max) + " WHERE table_name = :table_name";
    query.prepare(queryText);
    query.bindValue(":table_name", tableName);
    if(!query.exec())
    {
        qDebug() << query.lastError().text();
    }
}

