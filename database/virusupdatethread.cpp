#include "virusupdatethread.h"
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include "dicemaster.h"
#include <QDebug>

VirusUpdateThread::VirusUpdateThread(QSqlQuery &query, QString tableName,
    QList<double> &before, QList<double> &update, QObject* parent)
    : QThread(parent),
      tableName(tableName),
      query(query),
      before(before),
      update(update)
{
}

VirusUpdateThread::~VirusUpdateThread()
{
}

void VirusUpdateThread::run()
{
    QList<double> allNumbersTogether = before + update;
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

    // Read stage.
    int j = 1;
    QList<int> ids;
    for(int i = 0; i < scaledBefore.length(); i += scaledBefore.size() / update.size())
    {
        int number = scaledBefore[i];
        emit onProgress(j / (update.size() * 1.0) * 100);
        query.prepare(QString("SELECT id FROM ") + tableName + " WHERE value=" + QString::number(number));
        if(!query.exec())
        {
            qDebug() << query.lastQuery();
            qDebug() << query.lastError().text();
        }
        if(!query.next())
        {
            Q_ASSERT(false);
        }
        ids << query.value(0).toInt();
        j++;
    }

    // Virus update.
    i = 1;
    QList<QPair<int, int>> idToScaled;
    QList<int> scaledUpdate = DiceMaster::scaleListUp(update);
    auto a = ids.begin();
    auto b = scaledUpdate.begin();
    for(; a != ids.end() && b != scaledUpdate.end(); a++, b++)
    {
        idToScaled << qMakePair(*a, *b);
    }
    qDebug() << "size" << idToScaled.size();
    for(auto idScaled : idToScaled)
    {
        if(i % (idToScaled.size() / 100) == 0)
        {
            emit onProgress((i * 1.0) / idToScaled.size() * 100);
        }
        qDebug() << "value" << idScaled.second;
        qDebug() << "id" << idScaled.first;
        query.prepare(QString("UPDATE ") + tableName + " set value = " + QString::number(idScaled.second)
                      + " WHERE id = " + QString::number(idScaled.first));
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
        + QString::number(count - update.size()) + ", virus_count = "
        + QString::number(update.size()) + ", min = " + QString::number(min) + ", max = "
        + QString::number(max) + ", status = :status WHERE table_name = :table_name";
    query.prepare(queryText);
    query.bindValue(":status", "vinsert");
    query.bindValue(":table_name", tableName);
    if(!query.exec())
    {
        qDebug() << query.lastError().text();
    }
}
