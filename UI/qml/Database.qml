import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.3
import "common/"

Page {
  id: databasePage
  width: 600
  height: 400

  title: qsTr("Data-base")

  ColumnLayout {
    height: databasePage.height
    width: databasePage.width
    LoadButton {
      id: loadButton
      title: qsTr("Load Database")
      dialogTitle: qsTr("Please choose a database file")
      Layout.fillWidth: true
      onAccepted: runModel.loadFile(url)
    }
    RowLayout {
      Layout.fillWidth: true
      TcButton {
        title: qsTr("Add Beamtime")
        Layout.fillWidth: true
        enabled: true
        onClicked: runModel.addBeamtime()
      }
      TcButton {
        title: qsTr("Remove beamtime")
        Layout.fillWidth: true
        enabled: listView.currentIndex >= 0
        onClicked: runModel.removeBeamtime(listView.currentIndex)
      }
    }
    RowLayout {
      Layout.fillWidth: true
      TcButton {
        title: qsTr("Add run")
        Layout.fillWidth: true
        enabled: listView.currentIndex >= 0
        onClicked: listView.currentItem.beamtime.addRun()
      }
      TcButton {
        title: qsTr("Remove run")
        Layout.fillWidth: true
        enabled: listView.currentItem.current >= 0
        onClicked: listView.currentItem.beamtime.removeRun(listView.currentItem.current)
      }
    }

    RowLayout {
      Layout.fillHeight: true
      Layout.fillWidth: true
      TcListView {
        id: listView
        Layout.fillHeight: true
        width: databasePage.width * 0.4
        delegate: BeamtimeDelegate {
          width: listView.width
          property variant beamtime: model
        }
        model: runModel
      }
      ColumnLayout{
        Layout.fillHeight: true
        width: databasePage.width * 0.6
        BeamtimeDisplay{
          Layout.fillHeight: true
          Layout.fillWidth: true
          object: listView.currentItem.beamtime
        }
        RunDisplay{
          Layout.fillHeight: true
          Layout.fillWidth: true
          object: listView.currentItem.currentRun
        }
      }
    }
    spacing: Common.spacing
  }
}
