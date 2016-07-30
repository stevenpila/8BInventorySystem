import QtQuick 2.5
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import "qrc:/js/ISHelperScript.js" as ISScript

Window {
    title: qsTr("8 Brother\'s Inventory System")
    visible: true
    width: 640
    height: 480

    //! to prevent resizing window
    minimumWidth: width
    minimumHeight: height
    maximumWidth: width
    maximumHeight: height

    MouseArea {
        anchors.fill: parent
        onClicked: {
            ISQmlController.sampleSlot();
//            Qt.quit();
        }
    }

    Connections {
        target: ISQmlController
        onSample: {
            console.log("onSample triggered")
        }
    }

    TabView {
        id: frame
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        anchors.fill: parent

        Tab {
            title: "home"

            ColumnLayout {
                id: homeLayout
                anchors.fill: parent

                TableView {
                    id: inventoryTable
                    anchors.fill: parent

                    sortIndicatorVisible: true
                    frameVisible: false

                    TableViewColumn {
                        id: idColumn
                        title: "ID"
                        role: "id"
                        movable: false
                        resizable: false
                        width: 100
                    }

                    TableViewColumn {
                        id: itemColumn
                        title: "Item"
                        role: "item"
                        movable: false
                        resizable: false
                        width: inventoryTable.viewport.width - idColumn.width - quantityColumn.width - priceColumn.width
                    }

                    TableViewColumn {
                        id: quantityColumn
                        title: "Quantity"
                        role: "quantity"
                        movable: false
                        resizable: false
                        width: 100
                    }

                    TableViewColumn {
                        id: priceColumn
                        title: "Price"
                        role: "price"
                        movable: false
                        resizable: false
                        width: 100
                    }
                }
            }
        }
        Tab {
            title: "report"
            Rectangle {
                color: "orange"
            }
        }

        style: TabViewStyle {
            frameOverlap: 1
            tab: Rectangle {
                color: styleData.selected ? "steelblue" : "lightsteelblue"
                border.color:  "steelblue"
                implicitWidth: (styleData.availableWidth / 2) + 1 // Math.max(text.width + 4, 80)
                implicitHeight: 45
                radius: 0

                Image {
                    id: image
                    anchors.centerIn: parent
                    source: ISScript.getIcon(styleData.title, styleData.selected)
                }
            }
//            frame: Rectangle { color: "steelblue" }
        }
    }
}
