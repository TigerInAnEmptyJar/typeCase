import QtQuick 2.12
import QtQuick.Controls 2.5
import "common/"

Item {
  id: runDelegate
  Text {
    id: text1
    x: Common.textMargin
    height: Common.textHeight
    text: model.name
  }
}
