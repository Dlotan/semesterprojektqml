#ifndef VIRUSINSERTTHREAD_H
#define VIRUSINSERTTHREAD_H

#include <QObject>
#include <QThread>
#include <QList>
#include <QSqlQuery>

class VirusInsertThread : public QThread
{
    Q_OBJECT
public:
    VirusInsertThread(QSqlQuery &query, QString tableName,
        QList<double> &before, QList<double> &after, QObject *parent = nullptr);
    ~VirusInsertThread();

    void run() override;
signals:
    void onProgress(int);
private:
    QString tableName;
    QSqlQuery query;
    QList<double> before;
    QList<double> after;
};

#endif // VIRUSINSERTTHREAD_H
