import QtQuick 1.0
import com.nokia.meego 1.0

Rectangle {
    property alias title: heading.text
    smooth: true
    height: 55
    width: parent.width
    color: "#1F3C47"

    Image {
        id: togglLogo
        source: "toggl.png"
        height: 40
        fillMode: Image.PreserveAspectFit
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.topMargin: 5
    }
    Text {
        anchors.topMargin: 20
        anchors.rightMargin: 16
        anchors.leftMargin: 16
        anchors.left: togglLogo.right
        width: 0.8 * parent.width
        id: heading
        color: "#ffffff"
        font.pixelSize: 32
    }

    MouseArea {
      anchors.fill: parent
    }
}
