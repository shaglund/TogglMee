import QtQuick 1.1
import com.nokia.meego 1.0
import QtMobility.feedback 1.1

Page {
    property variant entryModel

    id: mainPage

    Header {
        id: header
        title: "TogglMee"
        anchors.top: parent.top
        anchors.left: parent.left
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
                width: 5*parent.width/8
                height: parent.height
                anchors {
                    left: parent.left
                    leftMargin: 2
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
                width: 2*parent.width/8
                height: parent.height
                anchors.left: descCol.right
                Text {
                    id: timeDurationText
                    color: "darkgrey"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: parent.width
                    horizontalAlignment: Text.AlignHCenter
                    text: duration
                    elide: Text.ElideRight
                    font.pixelSize: 20
                    font.family: "Arial"
                }
            }
            Rectangle {
                id: continueCol
                color: "#EAF4FE"
                smooth: true
                width: parent.width/8
                height: parent.height
                anchors.left: durationCol.right
                ToolIcon {
                    platformIconId: "toolbar-mediacontrol-play"

                    anchors {
                        right: parent.right
                        rightMargin: -6
                        verticalCenter: parent.verticalCenter
                        verticalCenterOffset: -6
                    }
                }
            }
            MouseArea {
                anchors.fill: descCol
                onPressAndHold: {
                    rumbleEffect.start();
                    queryDelete.model = entryModel
                    queryDelete.index = index
                    queryDelete.open();
                }
                onClicked: {
                }
            }
            MouseArea {
                anchors.fill: durationCol
                onClicked: {
                    entryModel.continueTimeEntry(index);
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
                placeholderText: entryModel.togglDescription
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
                text: entryModel.togglDuration;
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
                text: entryModel.togglText;
                font.pixelSize: 25
                onClicked: entryModel.toggl(taskText.text);
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
            model: entryModel
            delegate: mydelegate
            section.property: "date"
            section.criteria: ViewSection.FullString
            section.delegate: sectionHeading
            header: togglHeader
        }
    }
}
