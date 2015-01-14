#ifndef INSERTTHREAD_H
#define INSERTTHREAD_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QList>
#include <QThread>

class InsertThread : public QThread
{
    Q_OBJECT
public:
    InsertThread(QSqlQuery& query, QString tableName, QList<double>& numbers, QObject *parent = nullptr);
    void run() override;
    ~InsertThread();
signals:
    void onProgress(int);
private:
    QSqlQuery query;
    QString tableName;
    QList<double> numbers;
};

#endif // INSERTTHREAD_H
