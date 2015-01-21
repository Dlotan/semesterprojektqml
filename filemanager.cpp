#include "filemanager.h"

#include <QFile>
#include <QVector>

QTextStream FileManager::outStream;

FileManager::FileManager()
{

}

FileManager::~FileManager()
{

}

void FileManager::initialize()
{
    QString fileName("out.csv");
    QFile::remove(fileName);
    QFile* file = new QFile(fileName);
    if (!file->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }
    outStream.setDevice(file);
    outStream << "blub";
    outStream.flush();
}

void FileManager::write(QString tableName, QVariantList averages, QVector<double> stdDeviations)
{
    outStream << tableName << ";" << "averages;";
    for(QVariant average : averages)
    {
        outStream << average.toInt() << ";";
    }
    outStream << "stdDeviations;";
    for(double stdDeviation : stdDeviations)
    {
        outStream << static_cast<int>(stdDeviation) << ";";
    }
    outStream << "\n";
    outStream.flush();
}

