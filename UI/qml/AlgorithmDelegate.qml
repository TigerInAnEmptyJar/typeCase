import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

import "common/"

Item {
  id: algorithmDelegate
  property ListView listView
  property bool isActive : listView.currentIndex == index
  property bool readonly : false
  height: isActive ? Common.textHeight + valuesView.count * Common.textHeight : Common.textHeight
  MouseArea {
    anchors.fill: parent
    onClicked: algorithmDelegate.listView.currentIndex = index
  }
  ColumnLayout {
    Text {
      id: text1
      text: name
      Layout.fillWidth: true
    }
    TcValuesView {
      id: valuesView
      visible: algorithmDelegate.isActive
      Layout.fillWidth: true
      Layout.fillHeight: true
      readonly: algorithmDelegate.readonly
      model: values
    }
  }
}
