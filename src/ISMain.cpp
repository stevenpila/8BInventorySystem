#include <QQuickView>
#include <QGuiApplication>
#include <QQmlContext>

#include <ISQmlController.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickView view;

    ISQmlController isQmlController;
    view.rootContext()->setContextProperty("ISQmlController", &isQmlController);

    view.setSource(QUrl(QStringLiteral("qrc:/qml/ISHomeScreen.qml")));
    view.show();

    return app.exec();
}
