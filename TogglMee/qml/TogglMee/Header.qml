import QtQuick 1.0
import com.nokia.meego 1.0

Rectangle {
    property alias title: heading.text
    property alias user: menuText.text

    smooth: true
    height: 55
    width: parent.width
    anchors.top: parent.top
    color: "#1F3C47"

    Image {
        id: togglLogo
        source: "toggl.png"
        height: 40
        fillMode: Image.PreserveAspectFit
        anchors {
            top: parent.top
            left: parent.left
            leftMargin: 10
            topMargin: 5
        }
    }
    Text {
        anchors {
            top: parent.top
            left: togglLogo.right
            leftMargin: 10
            topMargin: 5
        }
        // width: 0.8 * parent.width
        id: heading
        color: "#ffffff"
        font.pixelSize: 32
    }
    BusyIndicator {
        id: busy
        anchors {
            top: parent.top
            left: heading.right
            leftMargin: 10
            topMargin: 10
        }
        height: 50
        running: loader.isBusy
        visible: loader.isBusy
    }
    Rectangle {
        id: headerMenu
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: busy.right
            right: parent.right
            leftMargin: 20
            rightMargin: 20
            topMargin: 7
            bottomMargin: 7
        }
        radius: 5.0
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#50626b" }
            GradientStop { position: 0.5; color: "#0F1C23" }
        }
        Text {
            id: menuText
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 10
            text: loader.userName
            font.pixelSize: 18
            color: "white"
        }
    }

    MouseArea {
        anchors.fill: headerMenu
        onClicked: selectionDialog.open();
    }

    Menu {
        id: selectionDialog
        anchors.top: headerMenu.bottom
        anchors.left: headerMenu.left
        anchors.right: headerMenu.right
        MenuLayout {
            MenuItem {
                text: "Log out"
                onClicked: loader.setApiKey("");
            }
            MenuItem {
                text: "Console"
                onClicked: appWindow.pageStack.push(messagePage);
            }
            MenuItem {
                text: "Back"
                onClicked: appWindow.pageStack.pop();
            }
        }
    }
}
