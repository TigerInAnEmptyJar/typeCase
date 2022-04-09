import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import "common/"

Rectangle{
  property variant object: undefined

  visible: object != undefined

  height: 4 * Common.textHeight + 4 * Common.controlHeight
  border {
    color: "darkgreen"
    width: 1
  }

  ColumnLayout {
    anchors.fill: parent
    RowLayout{
      Layout.fillWidth: true
      Text {
        id: nameId
        height: Common.textHeight
        text: qsTr("Run:")
      }
      TextInput {
        height: Common.textHeight
        text: object.name
        onTextChanged: object.name = text
      }
    }
    RowLayout {
      Text {
        height: Common.textHeight
        text: qsTr("Id:")
        Layout.fillWidth: true
      }
      TextInput{
        height: Common.textHeight
        text: object.identifier
        validator: IntValidator{
          bottom: 0
          top: 100000000
        }
        onTextChanged: object.identifier = text
      }
    }
    RowLayout {
      Layout.fillWidth: true
      Text {
        height: Common.controlHeight
        text: qsTr("Type")
      }
      ComboBox {
        id: typeBox
        implicitHeight: Common.controlHeight
        Layout.fillWidth: true
        model: [qsTr("Regular"), qsTr("Calibration"), qsTr("Elastic"), qsTr("Background"), qsTr("Other")]
        currentIndex: object.type
        onActivated: object.type = currentIndex
      }
    }
    RowLayout {
      Layout.fillWidth: true
      Text {
        height: Common.controlHeight
        text: "Start date"
      }
      TextInput {
        height: Common.controlHeight
        Layout.fillWidth: true
        text: object.start
        inputMask: "00.00.0000-00:00:00"
        onTextChanged: object.start = text
        color: acceptableInput ? Common.goodColor : Common.errorColor
      }
    }
    RowLayout {
      Layout.fillWidth: true
      Text {
        height: Common.controlHeight
        text: qsTr("Stop date")
      }
      TextInput {
        height: Common.controlHeight
        Layout.fillWidth: true
        text: object.stop.toLocaleString()
        inputMask: "00.00.0000-00:00:00"
        onTextChanged: object.stop = text
        color: acceptableInput ? Common.goodColor : Common.errorColor
      }
    }
    RowLayout{
      height: Common.controlHeight
      Layout.fillWidth: true
      CheckBox {
        id: hasOwnSetup
        implicitHeight: Common.controlHeight
        text: qsTr("Own setup")
        checked: object.ownSetup
        onCheckedChanged: object.ownSetup = checked
      }
      Text {
        text: object.setupfile
        height: Common.controlHeight
        Layout.fillWidth: true
        visible: hasOwnSetup.checked
      }
      LoadButton {
        enabled: hasOwnSetup.checked
        height: Common.controlHeight
        width: Common.controlHeight * 2
        title: qsTr("Setup")
        dialogTitle: qsTr("Please choose a setup file")
        onAccepted: object.setupfile = url
      }
    }
    CheckBox {
      text: qsTr("Own calibration")
      implicitHeight: Common.controlHeight
      Layout.fillWidth: true
      checked: object.ownCalibration
      onCheckedChanged: object.ownCalibration = checked
    }
    CheckBox {
      text: qsTr("Additional calibration")
      implicitHeight: Common.controlHeight
      Layout.fillWidth: true
      checked: object.additionalCalibration
      onCheckedChanged: object.additionalCalibration = checked
    }
  }
}
