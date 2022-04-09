import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.3

Item {
  id: loadButton
  property string dialogTitle
  property string title
  signal accepted(string url)

  height: button.height

  TcButton {
    id: button
    title: loadButton.title
    width: parent.width
    FileDialog {
      id: fileDialog
      title: loadButton.dialogTitle
      onAccepted: {
        loadButton.accepted(fileDialog.fileUrl)
        visible = false
      }
      onRejected: {
        visible = false
      }
    }
    onClicked: {
      fileDialog.visible = true
    }
  }
}
