import QtQuick 2.0

import "qrc:/js/ISHelperScript.js" as ISScript

Row {
    id: tabContainerId

    height: 80
    anchors.top: parent.top
    anchors.left: parent.left

    Repeater {
        id: tabRepeaterId

        model: [
            { title: "home", source: "qrc:/qml/ISHomeTabContent.qml" },
            { title: "report", source: "qrc:/qml/ISReportTabContent.qml" }
        ]

        Rectangle {
            id: tabId

            property bool isPressed: (modelData.title === "home") ? true : false
            property alias title: tabTextId.text

            width: 150
            height: parent.height
            color: tabId.isPressed ? "steelblue" : "lightsteelblue"

            Text {
                id: tabTextId

                text: modelData.title
                color: "transparent"
            }

            Image {
                id: tabImageId

                anchors.centerIn: parent
                source: ISScript.getIcon(tabTextId.text, tabId.isPressed)
            }

            MouseArea {
                id: tabMouseAreaId

                anchors.fill: parent
                onClicked: {
                    if(!tabId.isPressed) {
                        tabId.isPressed = true;

                        for(var i = 0; i < tabRepeaterId.count; ++i) {
                            if(tabRepeaterId.itemAt(i).title !== modelData.title) {
                                tabRepeaterId.itemAt(i).isPressed = false;
                            }
                        }

                        // load tab content in loader here...
                        tabContentLoaderId.setSource(modelData.source);
                    }
                }
            }
        }
    }
}
