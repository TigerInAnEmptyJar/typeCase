import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import "common/"

Page {
  id: setupPage
  width: 600
  height: 400

  title: qsTr("Detector Setup")

  ColumnLayout {
    height: setupPage.height
    width: setupPage.width
    spacing: Common.spacing
    LoadButton {
      id: loadButton
      height: Common.controlHeight
      title: qsTr("Load Setup")
      dialogTitle: qsTr("Please choose a setup file")
      Layout.fillWidth: true
      Layout.fillHeight: false
      onAccepted: detectorModel.loadFile(url)
    }
    RowLayout {
      Layout.fillHeight: false
      Layout.fillWidth: true
      height: Common.controlHeight
      spacing: Common.spacing
      TcButton {
        id: addButton
        height: Common.controlHeight
        title: qsTr("Add Detector")
        Layout.fillWidth: true
        enabled: true
        onClicked: detectorModel.add()
      }
      TcButton {
        title: qsTr("Remove Detector")
        Layout.fillWidth: true
        enabled: listView.currentIndex >= 0
        onClicked: detectorModel.remove(listView.currentIndex)
      }
    }
    TcListView {
      id: listView
      Layout.fillHeight: true
      Layout.fillWidth: true
      delegate: DetectorDelegate {
        width: listView.width
      }
      model: detectorModel
      clip: true
    }
  }
}
