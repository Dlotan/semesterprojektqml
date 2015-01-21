#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QVariantList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = 0);
    Q_INVOKABLE void createTable(QString tableName, QString distribution, bool hasIndex);
    Q_INVOKABLE void deleteTable(QString tableName);
    Q_INVOKABLE void resetDatabase();
    Q_INVOKABLE bool fillTable(QString tableName, int quantity, int initialClasses);
    Q_INVOKABLE QVariantList getNumbers(QString tableName);
    QList<int> profileQuery(QString queryString);
    int closest(const QList<int>& numbers, int search);
    QVariantList profileTableSingle(QString tableName);
    QVariantList profileTableRange(QString tableName);
    Q_INVOKABLE QVariantMap profile(bool range);
    Q_INVOKABLE bool virusInsert(QString tableName, QString virusDistribution, int quantity, int initialClasses);
    Q_INVOKABLE bool virusUpdate(QString tableName, QString virusDistribution, int quantity, int initialClasses);
    ~Database();

signals:
    void onError(QString);
    void onProgress(int);
    void onFillFinished();

public slots:

private:
    QSqlDatabase permDatabaseConnection;
    QSqlQuery query;
};

#endif // DATABASE_H
