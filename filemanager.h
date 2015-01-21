#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QTextStream>
#include <QVariantList>
#include <QVector>
#include <QString>
#include <QFile>

class FileManager
{
public:
    FileManager();
    ~FileManager();
    static void initialize();
    static void write(QString tableName, QVariantList averages, QVector<double> stdDeviations);
private:
    static QTextStream outStream;
};

#endif // FILEMANAGER_H
