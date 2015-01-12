#include "sqlquerymodel.h"
#include <QSqlRecord>
#include <QSqlField>
#include <QDebug>

SqlQueryModel::SqlQueryModel(QObject *parent)
    : QSqlQueryModel(parent)
{

}

SqlQueryModel::~SqlQueryModel()
{

}

void SqlQueryModel::setQuery(const QString& query)
{
    QSqlQueryModel::setQuery(query);
}

QString SqlQueryModel::getName(int index)
{
    QModelIndex modelIndex = this->index(index, 1);
    return QSqlQueryModel::data(modelIndex, Qt::DisplayRole).toString();
}

QHash<int, QByteArray> SqlQueryModel::roleNames() const
{
    QHash<int, QByteArray> roleNames;
    for( int i = 0; i < record().count(); i++) {
        roleNames[Qt::UserRole + i + 1] = record().fieldName(i).toLatin1();
    }
    return roleNames;
}

QVariant SqlQueryModel::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlQueryModel::data(index, role);
    if(role < Qt::UserRole)
    {
        value = QSqlQueryModel::data(index, role);
    }
    else
    {
        int columnIdx = role - Qt::UserRole - 1;
        QModelIndex modelIndex = this->index(index.row(), columnIdx);
        value = QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
    }
    return value;
}
