import QtQuick 1.1
import com.nokia.meego 1.0
import QtMobility.feedback 1.1

Page {
    Header {
        id: header
        title: "TogglMee"
        anchors.top: parent.top
        anchors.left: parent.left
    }

    Rectangle {
        width: parent.width
        height: parent.height - header.height
        anchors.top: header.bottom
        color: "#E8E8E8"
    }
}
