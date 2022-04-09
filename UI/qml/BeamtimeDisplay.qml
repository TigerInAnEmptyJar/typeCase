import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import "common/"

Rectangle{
  property variant object: none

  height: 3 * Common.controlHeight
  ColumnLayout {
    anchors.fill: parent
    RowLayout {
      Layout.fillHeight: true
      Layout.fillWidth: true
      Text {
        id: nameId
        height: Common.controlHeight
        text: qsTr("Beamtime:")
      }
      TextInput {
        height: Common.controlHeight
        text: object.name
        onTextChanged: object.name = text
      }
    }
    RowLayout {
      Layout.fillHeight: true
      Layout.fillWidth: true
      Text {
        height: Common.controlHeight
        text: qsTr("Month:")
      }
      SpinBox {
        implicitHeight: Common.controlHeight
        from: 1
        to: 12
        value: object.month
        onValueChanged: object.month = value
      }
      Text {
        height: Common.controlHeight
        text: qsTr("Year:")
      }
      SpinBox {
        implicitHeight: Common.controlHeight
        from: 1900
        to: 2500
        value: object.year
        onValueChanged: object.year = value
      }
    }
    RowLayout {
      Layout.fillHeight: true
      Layout.fillWidth: true
      Text {
        height: Common.controlHeight
        text: qsTr("Setup:")
      }
      Text {
        text: object.setupfile
        height: Common.controlHeight
        Layout.fillWidth: true
      }
      LoadButton {
        implicitHeight: Common.controlHeight
        width: Common.controlHeight * 2
        title: qsTr("Setup")
        dialogTitle: qsTr("Please choose a setup file")
        onAccepted: object.setupfile = url
      }
    }
  }
}
