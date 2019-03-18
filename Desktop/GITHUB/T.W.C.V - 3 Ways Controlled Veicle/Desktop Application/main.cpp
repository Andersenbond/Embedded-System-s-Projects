#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include "client.h"

int main(int argc, char *argv[])
{
    qmlRegisterType<Client>("Client", 1, 0, "Client");

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();

}

