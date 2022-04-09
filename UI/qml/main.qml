import QtQuick 2.12
import QtQuick.Controls 2.5
import "common/"

ApplicationWindow {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("TypeCase")

    header: ToolBar {
        contentHeight: toolButton.implicitHeight

        ToolButton {
            id: toolButton
            text: stackView.depth > 1 ? "\u25C0" : "\u2630"
            font.pixelSize: Qt.application.font.pixelSize * 1.6
            onClicked: {
                if (stackView.depth > 1) {
                    stackView.pop()
                } else {
                    drawer.open()
                }
            }
        }

        Label {
            text: stackView.currentItem.title
            anchors.centerIn: parent
        }
    }

    Drawer {
        id: drawer
        width: window.width * 0.66
        height: window.height

        Column {
            anchors.fill: parent

            ItemDelegate {
                text: qsTr("Algorithms")
                width: parent.width
                onClicked: {
                    stackView.push("Algorithms.qml")
                    drawer.close()
                }
            }
            ItemDelegate {
                text: qsTr("Database")
                width: parent.width
                onClicked: {
                    stackView.push("Database.qml")
                    drawer.close()
                }
            }
            ItemDelegate {
                text: qsTr("Setup")
                width: parent.width
                onClicked: {
                    stackView.push("Setup.qml")
                    drawer.close()
                }
            }
            ItemDelegate {
                text: qsTr("Materials")
                width: parent.width
                onClicked: {
                    stackView.push("Material.qml")
                    drawer.close()
                }
            }
            ItemDelegate {
                text: qsTr("Shapes")
                width: parent.width
                onClicked: {
                    stackView.push("Shapes.qml")
                    drawer.close()
                }
            }
        }
    }

    StackView {
        id: stackView
        initialItem: "HomeForm.ui.qml"
        anchors.fill: parent
    }
}
