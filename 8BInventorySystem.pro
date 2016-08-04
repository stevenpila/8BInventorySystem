TEMPLATE = app

QT += qml quick sql widgets
CONFIG += c++11

SOURCES += \
    src/controller/ISQmlController.cpp \
    src/ISMain.cpp \
    src/model/InventoryModel.cpp \
    src/utility/DatabaseManager.cpp

RESOURCES += \
    qml.qrc \
    js.qrc \
    img.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
#include(deployment.pri)

HEADERS += \
    hdr/controller/ISQmlController.h \
    hdr/model/InventoryModel.h \
    hdr/utility/LoggerUtility.h \
    hdr/utility/DatabaseManager.h \
    hdr/utility/Commons.h

INCLUDEPATH += \
    hdr/ \
    hdr/model/ \
    hdr/controller/ \
    hdr/utility/

DISTFILES += \
    qml/ISMainScreen.qml \
    qml/ISHomeScreen.qml \
    res/js/ISHelperScript.js \
    README.md \
    deployment.pri \
    qml/ISHomeTabContent.qml \
    qml/ISReportTabContent.qml \
    qml/ISHomeScreenTabs.qml
