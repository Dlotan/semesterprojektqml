#ifndef VIRUSUPDATETHREAD_H
#define VIRUSUPDATETHREAD_H


#include <QObject>
#include <QThread>
#include <QList>
#include <QSqlQuery>

class VirusUpdateThread : public QThread
{
    Q_OBJECT
public:
    VirusUpdateThread(QSqlQuery &query, QString tableName,
        QList<double> &before, QList<double> &update, QObject *parent = nullptr);
    ~VirusUpdateThread();

    void run() override;
signals:
    void onProgress(int);
private:
    QString tableName;
    QSqlQuery query;
    QList<double> before;
    QList<double> update;
};

#endif // VIRUSUPDATETHREAD_H
