import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.3
import "common/"

Page {
  id: materialPage
  width: 600
  height: 400

  title: qsTr("Materials")

  ColumnLayout {
    height: materialPage.height
    width: materialPage.width
    LoadButton {
      id: loadButton
      title: qsTr("Load Materials")
      dialogTitle: qsTr("Please choose a material file")
      Layout.fillWidth: true
      onAccepted: materialModel.loadFile(url)
    }
    RowLayout {
      TcButton {
        title: qsTr("Add Material")
        Layout.fillWidth: true
        enabled: true
        onClicked: materialModel.add()
      }
      TcButton {
        title: qsTr("Remove Material")
        Layout.fillWidth: true
        enabled: listView.currentIndex >= 0
        onClicked: materialModel.remove(listView.currentIndex)
      }
    }

    TcListView {
      id: listView
      Layout.fillHeight: true
      Layout.fillWidth: true
      delegate: MaterialDelegate {
        width: parent.width
      }
      model: materialModel
    }
    spacing: Common.spacing
  }
}
