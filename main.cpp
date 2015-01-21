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

#include "generator/generatornormal.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    std::srand(QTime::currentTime().msec());
    FileManager::initialize();
    QQmlApplicationEngine engine;
    //engine.addImportPath(QLatin1String("/Users/f14r/workspaces/qt/semesterprojektqml/qml");
    qDebug() << engine.importPathList();
    qDebug() << engine.baseUrl();
    qmlRegisterType<Database>("custom", 1, 0, "Database");
    qmlRegisterType<SqlQueryModel>("custom", 1, 0, "SqlQueryModel");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    return app.exec();
}
