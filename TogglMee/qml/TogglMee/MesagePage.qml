// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import com.nokia.meego 1.0

Page {

    tools: ToolBarLayout {
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: appWindow.pageStack.pop();
        }
    }

    Component {
        id: messageDelegate
        Rectangle {
            width: parent.width
            height: childrenRect.height
            Text {
                width: parent.width
                id: msgText
                text: "<b>" + display + "</b>"
                font.pixelSize: 20
                wrapMode: Text.WrapAnywhere
            }
        }
    }

    Rectangle {
        width: parent.width
        height: parent.height
        anchors.top: header.bottom
        color: "#E8E8E8"
        ListView {
            id: msgListView
            clip: true
            anchors.fill: parent
            model: messagesModel
            delegate: messageDelegate
        }
    }
}
