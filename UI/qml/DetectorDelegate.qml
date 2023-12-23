import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import "common/"

Item {
  id: detectorDelegate
  height: listView.currentIndex === index ? Common.textHeight + 6 * Common.controlHeight + shapeDisplay.preferedHeight :
                                           Common.textHeight
  MouseArea {
    anchors.fill: parent
    onClicked: listView.currentIndex = index
  }
  ColumnLayout {
    anchors.fill: parent
    Text {
      id: textName
      Layout.fillWidth: true
      height: Common.textHeight
      text: name
      font.bold: true
    }
    GridLayout {
      id: grid1
      visible: listView.currentIndex === index
      columns: 2
      Layout.fillHeight: true
      Layout.fillWidth: true
      Text {
        height: Common.controlHeight
        text: qsTr("Number of Elements")
      }
      TextInput {
        height: Common.controlHeight
        validator: IntValidator{top: 1000000; bottom: 0}
        text: elements
        onEditingFinished: elements = text
      }
      CheckBox {
        id: comboBox1
        implicitHeight: Common.controlHeight
        text: qsTr("Is Circular")
        checked: circular
        onCheckedChanged: circular = checked
      }
      Item {
        Layout.fillHeight: true
        Layout.fillWidth: true
      }
      Text {
        height: Common.controlHeight
        text: qsTr("Material")
      }
      ComboBox {
        id: comboBox2
        implicitHeight: Common.controlHeight
        model: materialModel
        delegate: Text{
          height: Common.controlHeight
          text:name
          MouseArea {
            anchors.fill:parent
            onClicked: {
              comboBox2.popup.close()
              material = index
            }
          }
        }
        displayText: material
        textRole: name
        flat: true
        onActivated: material = currentIndex
      }
      Text {
        height: Common.controlHeight
        text: qsTr("Maximal track distance")
      }
      TextInput {
        height: Common.controlHeight
        validator: DoubleValidator{notation: DoubleValidator.ScientificNotation; top: 1000000; bottom: 0}
        text: maxDistance
        onEditingFinished: maxDistance = text
      }
      Text {
        height: Common.controlHeight
        text: qsTr("Stack type")
      }
      TextInput {
        height: Common.controlHeight
        validator: IntValidator{top: 1000000; bottom: 0}
        text: stackType
        onEditingFinished: stackType = text
      }
      Text {
        height: Common.controlHeight
        text: qsTr("Shape")
      }
      ComboBox {
        id: comboBox3
        implicitHeight: Common.controlHeight
        model: shapeModel
        delegate: Text{
          text:name
          MouseArea {
            anchors.fill:parent
            onClicked: {
              comboBox3.popup.close()
              shapeType = index
            }
          }
        }
        displayText: shapeType
        textRole: name
        flat: true
        onActivated: shapeType = currentIndex
      }
    }
    ShapeDisplay {
      id: shapeDisplay
      Layout.fillWidth: true
      extended: listView.currentIndex == index
      visible: listView.currentIndex == index
      readonly: false
      name: shapeType
      values: shapeValues
    }
  }
}
