import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import "common/"

Page {
  id: shapePage
  width: 600
  height: 400

  title: qsTr("Defined Shapes")

  RowLayout {
    spacing: Common.spacing
    height: shapePage.height
    width: shapePage.width
    TcListView {
      id: listView
      Layout.fillHeight: true
      width: parent.width/3
      delegate: ShapeDelegate {
          property variant myData: model
          width: parent.width
      }
      model: shapeModel
    }

    Rectangle {
      id: shape
      Layout.fillHeight: true
      Layout.fillWidth: true
      Column {
        anchors.fill: parent
        Text {
          height: Common.textHeight
          text: listView.currentItem.myData.name
        }
        Text {
          id: description
          text: listView.currentItem.myData.description
        }
      }
    }
  }
}
