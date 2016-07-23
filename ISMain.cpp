#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QThread>

#include <ISQmlController.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    ISQmlController isQmlController;
    engine.rootContext()->setContextProperty("ISQmlController", &isQmlController);

    engine.load(QUrl(QStringLiteral("qrc:/ISMainScreen.qml")));

    return app.exec();
}
