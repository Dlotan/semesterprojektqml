#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QQmlContext>
#include <database/database.h>
#include <database/sqlquerymodel.h>

#include "dicemaster.h"
#include "filemanager.h"
#include <QDebug>
#include <cstdlib>
#include <QTime>

#define GUI

#include "generator/generatornormal.h"

#include <QRegularExpression>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    std::srand(QTime::currentTime().msec());
    FileManager::initialize();
#ifdef GUI
    QQmlApplicationEngine engine;
    //engine.addImportPath(QLatin1String("/Users/f14r/workspaces/qt/semesterprojektqml/qml");
    qDebug() << engine.importPathList();
    qDebug() << engine.baseUrl();
    qmlRegisterType<Database>("custom", 1, 0, "Database");
    qmlRegisterType<SqlQueryModel>("custom", 1, 0, "SqlQueryModel");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
#else
    Database *database = new Database();
    QString command = "";
    QTextStream qtin(stdin);
    while(true)
    {
        qtin >> command;
        if(!command.endsWith(")"))
        {
            command += "()";
        }
        command.remove(" ");
        QStringList arguments = command.remove(")").split("(");
        command = arguments[0];
        qDebug() << "command:" << command;
        arguments = arguments[1].split(",");
        qDebug() << "arguments:" << arguments;
        if(command == "exit")
        {
            break;
        }
        else if(command == "list")
        {
            database->list();
        }
        else if(command == "new" || command == "create")
        {
            if(arguments.length() < 2)
            {
                qDebug() << "wrong command";
                qDebug() << "create(name,distribution)";
            }
            database->createTable(arguments[0], arguments[1], true);
        }
        else if(command = "parasite")
        {
            qDebug() << "Commands are parasiteInsert or parasiteUpdate";
        }
        else if(command = "parasiteInsert")
        {
            if(arguments.length() < 4)
            {
                qDebug() << "wrong command";
                qDebug() << "parasiteInsert(name,parasiteDistribution,amount,initialclasses)";
            }
            database->virusInsert(arguments[0],arguments[1],arguments[2].toInt(),arguments[3].toInt());
        }
        else if(command = "parasiteUpdate")
        {
            if(arguments.length() < 4)
            {
                qDebug() << "wrong command";
                qDebug() << "parasiteUpdate(name,parasiteDistribution,amount,initialclasses)";
            }
            database->virusInsert(arguments[0],arguments[1],arguments[2].toInt(),arguments[3].toInt());
        }
        else if(command == "delete")
        {
            if(arguments.length() < 1)
            {
                qDebug() << "wrong command";
                qDebug() << "delete(name)";
            }
            database->deleteTable(arguments[0]);
        }
        else if(command == "fill")
        {
            if(arguments.length() < 3)
            {
                qDebug() << "wrong command";
                qDebug() << "fill(name,amount,initialclasses)";
            }
            database->fillTable(arguments[0], arguments[1].toInt(), arguments[2].toInt());
        }
        else if(command == "profileSingle")
        {
            database->profile(false);
        }
    }
    qDebug() << "finished";
#endif
    return app.exec();
}
