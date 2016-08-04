import QtQuick 2.0

Item {
    id: homeScreenId

    width: 640
    height: 480

    ISHomeScreenTabs {
        id: homeScreenTabsId

        property alias tabContentLoaderId: tabContentLoaderId
    }

    Loader {
        id: tabContentLoaderId

        anchors.top: homeScreenTabsId.bottom
        anchors.bottom: parent.bottom
        anchors.left : parent.left
        anchors.right: parent.right

        source: "qrc:/qml/ISHomeTabContent.qml" // default
    }
}
