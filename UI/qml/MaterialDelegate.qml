import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import "common/"

Item {
  id: materialDelegate
  property bool isActive: listView.currentIndex == index
  height: isActive ? Common.textHeight * 2 + 4 * Common.controlHeight :
                     Common.textHeight
  MouseArea {
    anchors.fill: parent
    onClicked: listView.currentIndex = index
    onDoubleClicked: {
      nameDisplay.visible = false
      textName.visible = true
    }
  }
  ColumnLayout {
    anchors.fill: parent
    TextInput {
      id: textName
      Layout.fillWidth: true
      height: Common.textHeight
      visible: false
      text: name
      font.bold: true
      onEditingFinished: {
        visible = false
        name = text
        nameDisplay.visible = true
        nameDisplay.text = name
      }
    }
    Text{
      id: nameDisplay
      Layout.fillWidth: true
      height: Common.textHeight
      text: name
      font.bold: true
    }
    GridLayout {
      id: grid1
      visible: materialDelegate.isActive
      Layout.fillHeight: true
      Layout.fillWidth: true
      columns: 2
      CheckBox {
        id: comboBox1
        implicitHeight: Common.controlHeight
        height: Common.controlHeight
        text: qsTr("Is Active")
        checked: active
        onClicked: active = checked
      }
      Item {
        Layout.fillHeight: true
        Layout.fillWidth: true
      }
      Text {
        height: Common.controlHeight
        text: qsTr("Radiation length")
      }
      TextInput {
        height: Common.controlHeight
        validator: DoubleValidator {
          notation: DoubleValidator.ScientificNotation
          top: 1000000
          bottom: 0
        }
        text: radiationLength
        onEditingFinished: radiationLength = text
      }
      Text {
        height: Common.controlHeight
        text: qsTr("Density")
      }
      TextInput {
        height: Common.controlHeight
        validator: DoubleValidator {
          notation: DoubleValidator.ScientificNotation
          top: 1000000
          bottom: 0
        }
        text: density
        onEditingFinished: density = text
      }
      Text {
        height: Common.controlHeight
        text: qsTr("Speed of Light")
      }
      TextInput {
        height: Common.controlHeight
        validator: DoubleValidator {
          notation: DoubleValidator.ScientificNotation
          top: 1000000
          bottom: 0
        }
        text: speedOfLight
        onEditingFinished: speedOfLight = text
      }
    }
  }
}
