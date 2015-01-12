#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QQmlContext>
#include <database/database.h>
#include <database/sqlquerymodel.h>

#include "dicemaster.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    //engine.addImportPath(QLatin1String("/Users/f14r/workspaces/qt/semesterprojektqml/qml");
    qDebug() << engine.importPathList();
    qDebug() << engine.baseUrl();
    qmlRegisterType<Database>("custom", 1, 0, "Database");
    qmlRegisterType<SqlQueryModel>("custom", 1, 0, "SqlQueryModel");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    // DiceMaster::getRandomNumbers("cauchy", 10000, 100000);
    return app.exec();
}
