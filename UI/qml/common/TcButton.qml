import QtQuick 2.12
import QtQuick.Controls 2.5

Rectangle {
    id: button
    property string title
    property bool enabled: true
    signal clicked

    height: Common.controlHeight
    color: Common.buttonColor
    Label {
        text: title
        anchors.centerIn: parent
    }
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked:
            if(button.enabled){
                button.clicked()
            }
    }
}
