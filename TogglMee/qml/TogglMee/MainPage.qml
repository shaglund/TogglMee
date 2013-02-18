import QtQuick 1.1
import com.nokia.meego 1.0
import QtMobility.feedback 1.1

Page {
    // tools: commonTools
    id: mainPage

    Header {
        id: header
        title: "TogglMee"
    }

    HapticsEffect {
         id: rumbleEffect
         attackIntensity: 0.0
         attackTime: 250
         intensity: 1.0
         duration: 100
         fadeTime: 250
         fadeIntensity: 0.0
     }

    QueryDialog {
        property int index
        property variant model
        id: queryDelete
        titleText: "Delete"
        message: "Delete this time entry?"
        acceptButtonText: "OK"
        rejectButtonText: "Cancel"
        onAccepted: model.deleteTimeEntry(index);
    }

    Component {
        id: mydelegate
        Item {
            property variant entry: model
            id: timeEntry
            width: parent.width
            height: 80
            Rectangle {
                id: descCol
                color: "#F5FAFF"
                smooth: true
                width: 2*parent.width/3
                height: parent.height
                anchors {
                    left: parent.left
                    leftMargin: 10
                }
                Text {
                    id: timeEntryText
                    color: "black"
                    anchors.verticalCenter: parent.verticalCenter
                    text: "<b>"+description+"</b><br>"+project
                    elide: Text.ElideRight
                    style: Text.Raised
                    font.pixelSize: 20
                    maximumLineCount: 1
                }
            }
            Rectangle {
                id: durationCol
                color: "#F5FAFF"
                smooth: true
                width: parent.width/3
                height: parent.height
                anchors {
                    left: descCol.right
                    right: parent.right
                    rightMargin: 10
                }
                Text {
                    id: timeDurationText
                    color: "darkgrey"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: duration
                    elide: Text.ElideRight
                    font.pixelSize: 22
                }
            }

            ToolIcon {
                platformIconId: "toolbar-next"
                anchors {
                    right: parent.right
                    rightMargin: -6
                    verticalCenter: parent.verticalCenter
                    verticalCenterOffset: -6
                }
            }
            MouseArea {
                anchors.fill: descCol
                onPressAndHold: {
                    rumbleEffect.start();
                    queryDelete.model = timeentryModel
                    queryDelete.index = index
                    queryDelete.open();
                }
                onClicked: {
                }
            }
            MouseArea {
                anchors.fill: durationCol
                onClicked: {
                    timeentryModel.continueTimeEntry(index);
                }
            }
        }
    }

    Component {
        id: sectionHeading
        Rectangle {
            width: parent.width
            height: childrenRect.height
            color: "#E8E8E8"
            Text {
                id: sectionText
                anchors {
                    left: parent.left
                    right: parent.right
                    leftMargin: 10
                    rightMargin: 10
                }
                color: "black"
                text: section
                font.bold: true
                font.pixelSize: 20
            }
        }
    }

    Component {
        id: togglHeader
        Rectangle {
            height: childrenRect.height + 20
            anchors {
                left: parent.left
                right: parent.right
                topMargin: 10
                bottomMargin: 10
            }
            color: "lightsteelblue"
            id: taskRow
            TextArea {
                id: taskText
                width: 2*parent.width/3
                placeholderText: timeentryModel.togglDescription
                anchors {
                    top: parent.top
                    left: parent.left
                    topMargin: 5
                    leftMargin: 20
                }
                height: 50
            }
            TextArea {
                id: taskDuration
                width: parent.width/3
                anchors {
                    top: parent.top
                    left: taskText.right
                    right: parent.right
                    topMargin: 5
                    leftMargin: 5
                    rightMargin: 20
                }
                readOnly: true
                text: timeentryModel.togglDuration;
                height: 50
            }
            Button {
                id: startStopButton
                height: 45
                anchors {
                    left: parent.left
                    right: parent.right
                    top:  taskText.bottom
                    leftMargin: 20
                    rightMargin: 20
                    topMargin: 5
                    bottomMargin: 5
                }
                text: timeentryModel.togglText;
                font.pixelSize: 25
                onClicked: timeentryModel.toggl(taskText.text);
            }
        }
    }


    Rectangle {
        width: parent.width
        height: parent.height - header.height
        anchors.top: header.bottom
        color: "#E8E8E8"
        ListView {
            id: listview
            clip: true
            anchors.fill: parent
            model: timeentryModel
            delegate: mydelegate
            section.property: "date"
            section.criteria: ViewSection.FullString
            section.delegate: sectionHeading
            header: togglHeader
        }
    }
}
