import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

TcListView {
  id: valuesView
  property bool readonly: true
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
        visible: valuesView.readonly
      }
      Text {
        text: value
        visible: !valuesView.readonly && !(type == "INT" || type == "FLOAT" ||
                                             type == "STRING" || type == "DOUBLE" ||
                                             type == "POINT3D" || type == "VECTOR3D" ||
                                             type == "BOOL")
      }
      TextInput {
        text: value
        visible: !valuesView.readonly && (type == "STRING")
        onEditingFinished: value = text
      }
      TextInput {
        text: value
        validator: DoubleValidator {
          top: 1000000000
          bottom: -100000000
        }
        visible: !valuesView.readonly && (type == "INT" || type == "FLOAT" ||
                                            type == "DOUBLE" || type == "BOOL")
        onEditingFinished: value = text
      }
      TextInput {
        text: value
        validator: RegExpValidator {
          regExp: /\([-]{0,1}[0-9]+(\.[0-9]+){0,1}(e[-|+]{0,1}[0-9]{0,3}){0,1}, [-]{0,1}[0-9]+(\.[0-9]+){0,1}(e[-|+]{0,1}[0-9]{0,3}){0,1}, [-]{0,1}[0-9]+(\.[0-9]+){0,1}(e[-|+]{0,1}[0-9]{0,3}){0,1}\)/
        }
        visible: !valuesView.readonly && (type == "POINT3D" || type == "VECTOR3D")
        onEditingFinished: value = text
        color: acceptableInput ? Common.goodColor : Common.errorColor
      }
      spacing: Common.spacing
    }
  }
}
