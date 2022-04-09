import QtQuick 2.12
import QtQuick.Controls 2.5
import "common/"

ShapeDisplay {
  id: shapeDelegate
  extended: listView.currentIndex == index
  readonly: true
  name: model.name
  values: model.values
  onClicked: listView.currentIndex = index
}
