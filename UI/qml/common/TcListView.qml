import QtQuick 2.12
import QtQuick.Controls 2.5

ListView {
  id: listView
  boundsBehavior: Flickable.StopAtBounds
  boundsMovement: Flickable.StopAtBounds
  highlightMoveDuration: 1
  highlightMoveVelocity: -1
  highlight: Rectangle {
    color: Common.highlightColor
    radius: Common.highlightRadius
  }
  keyNavigationEnabled: true
}
