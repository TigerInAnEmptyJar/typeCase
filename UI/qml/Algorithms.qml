import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import "common/"

Page {
  id: algorithmPage
    width: 600
    height: 400

    title: qsTr("Algorithms")

    ColumnLayout {
      height: algorithmPage.height
      width: algorithmPage.width
      spacing: Common.spacing
      LoadButton {
        id: loadButton
        height: Common.controlHeight
        title: qsTr("Load Algorithms")
        dialogTitle: qsTr("Please choose an algorithm file")
        Layout.fillWidth: true
        Layout.fillHeight: false
        onAccepted: algorithmModel.loadFile(url)
      }
      RowLayout {
        Layout.fillHeight: true
        Layout.fillWidth: true
        TcListView {
          id: listView1
          Layout.fillHeight: true
          Layout.fillWidth: true
          delegate: AlgorithmDelegate {
              property variant myData: model
            listView: listView1
              width: listView1.width
              readonly: true
          }
          model: algorithmModel
        }

        TcListView {
          id: listView2
          Layout.fillHeight: true
          Layout.fillWidth: true
          delegate: AlgorithmDelegate {
              property variant myData: model
            listView: listView2
              width: listView2.width
              readonly: false
          }
          model: algorithmModel
        }
      }
    }
}
