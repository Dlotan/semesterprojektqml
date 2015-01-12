#ifndef SQLQUERYMODEL_H
#define SQLQUERYMODEL_H

#include <QObject>
#include <QSqlQueryModel>

class SqlQueryModel : public QSqlQueryModel
{
    Q_OBJECT
protected:
    QHash<int, QByteArray> roleNames() const override;
public:
    explicit SqlQueryModel(QObject *parent = 0);
    ~SqlQueryModel();

    Q_INVOKABLE void setQuery(const QString &query);
    Q_INVOKABLE QString getName(int index);
    QVariant data(const QModelIndex &index, int role) const;
private:
};

#endif // TABLESMODEL_H
