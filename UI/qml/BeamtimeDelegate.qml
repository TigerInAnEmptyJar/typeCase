import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import "common/"

Item {
  id: beamtimeDelegate
  height: listView.currentIndex == index ? Common.textHeight + runsView.count * Common.textHeight :
                                           Common.textHeight
  property alias current: runsView.currentIndex
  property variant currentRun

  ColumnLayout {
    anchors.fill: parent
    Text {
      id: textName
      Layout.fillWidth: true
      height: Common.textHeight
      text: model.name
      MouseArea {
        anchors.fill: parent
        onClicked: listView.currentIndex = index
      }
    }

    TcListView{
      id: runsView
      Layout.fillHeight: true
      Layout.fillWidth: true
      delegate: RunDelegate {
        property variant selectedRun: model
        width: parent.width
        height: Common.textHeight
        MouseArea {
          anchors.fill: parent
          onClicked: {
            console.log(model.name)
            runsView.currentIndex = index
            beamtimeDelegate.currentRun = model
          }
        }
      }
      model: runs
      visible: listView.currentIndex == index
    }
    spacing: Common.spacing
  }
}
