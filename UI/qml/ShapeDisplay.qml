import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import "common/"

Item {
  id: shapeDisplay
  property bool extended: true
  property bool readonly: true

  property string name
  property var values
  signal clicked

  readonly property int preferedHeight: extended ? (valuesView.count + 1 ) * Common.controlHeight :
                                                   Common.controlHeight
  height: preferedHeight
  MouseArea {
    anchors.fill: parent
    onClicked: shapeDisplay.clicked()
  }

  ColumnLayout {
    anchors.fill: parent
    Text {
      id: text1
      height: Common.textHeight
      Layout.fillWidth: true
      text: name
      font.bold: true
    }
    TcListView {
      id: valuesView
      Layout.fillHeight: true
      Layout.fillWidth: true
      delegate: Item {
        width: valuesView.width
        height: Common.controlHeight
        RowLayout {
          anchors.fill: parent
          Text {
            text: name
          }
          Item {
            Layout.fillWidth: true
          }
          Text {
            text: type
            visible: shapeDisplay.readonly
          }
          Text {
            text: value
            visible: !shapeDisplay.readonly && !(type == "INT" || type == "FLOAT" ||
                                                 type == "STRING" || type == "DOUBLE" ||
                                                 type == "POINT3D" || type == "VECTOR3D" ||
                                                 type == "BOOL")
          }
          TextInput {
            text: value
            visible: !shapeDisplay.readonly && (type == "STRING")
            onEditingFinished: value = text
          }
          TextInput {
            text: value
            validator: DoubleValidator {
              top: 1000000000
              bottom: -100000000
            }
            visible: !shapeDisplay.readonly && (type == "INT" || type == "FLOAT" ||
                                                type == "DOUBLE" || type == "BOOL")
            onEditingFinished: value = text
          }
          TextInput {
            text: value
            validator: RegExpValidator {
              regExp: /\([-]{0,1}[0-9]+(\.[0-9]+){0,1}(e[-|+]{0,1}[0-9]{0,3}){0,1}, [-]{0,1}[0-9]+(\.[0-9]+){0,1}(e[-|+]{0,1}[0-9]{0,3}){0,1}, [-]{0,1}[0-9]+(\.[0-9]+){0,1}(e[-|+]{0,1}[0-9]{0,3}){0,1}\)/
            }
            visible: !shapeDisplay.readonly && (type == "POINT3D" || type == "VECTOR3D")
            onEditingFinished: value = text
            color: acceptableInput ? Common.goodColor : Common.errorColor
          }

          spacing: Common.spacing
        }
      }
      model: values
      visible: extended
    }
  }
}
