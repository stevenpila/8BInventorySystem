#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QThread>

#include <ISQmlController.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    ISQmlController isQmlController;
    engine.rootContext()->setContextProperty("ISQmlController", &isQmlController);

    engine.load(QUrl(QStringLiteral("qrc:/qml/ISMainScreen.qml")));

    return app.exec();
}
