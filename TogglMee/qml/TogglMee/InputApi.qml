// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import com.nokia.meego 1.1
import com.nokia.extras 1.1

Page {
    property bool uselogin: true

    InfoBanner {
        id: infoBanner
        z: 100
        text: ""
    }

    TextField {
        id: apiInput
        anchors.centerIn: parent
        anchors.left: parent.left
        anchors.right: parent.right
        width: parent.width - 20
        anchors.topMargin: 5
        anchors.bottomMargin: 5
        placeholderText: "API Key"
    }
    TextField {
        id: pwdInput
        visible: uselogin
        anchors.left: apiInput.left
        anchors.right: apiInput.right
        anchors.top: apiInput.bottom
        width: parent.width - 20
        anchors.topMargin: 5
        anchors.bottomMargin: 5
        echoMode: TextInput.PasswordEchoOnEdit
        placeholderText: "Password"
    }
    ButtonRow {
        anchors.left: apiInput.left
        anchors.right: apiInput.right
        anchors.top: uselogin ? pwdInput.bottom : apiInput.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        anchors.topMargin: 5
        Button {
            id: okButton
            text: "OK"
            onClicked: {
                if(!uselogin)
                    loader.setApiKey(apiInput.text);
                else
                    fetchApiKey(apiInput.text, pwdInput.text);
            }
        }
        Button {
            id: toggleLogin
            text: uselogin ? "Input Key" : "Login"
            onClicked: {
                uselogin = !uselogin
                apiInput.placeholderText = uselogin ? "email" : "API Key"
            }
        }
    }

    function fetchApiKey(user, passw) {
        okButton.enabled = false;
        var xhr = new XMLHttpRequest();
        var url = "https://www.toggl.com/api/v6/sessions.json";
        xhr.open("POST", url, true);
        xhr.setRequestHeader("Content-type","application/x-www-form-urlencoded");
        xhr.onreadystatechange = function()
                {
                    if ( xhr.readyState === xhr.DONE)
                    {
                        console.log("XMLHttpRequest DONE status=" + xhr.status);
                        if ( xhr.status === 200)
                        {
                            var jsonObject = eval('(' + xhr.responseText + ')');
                            loaded(jsonObject)
                        } else {
                            infoBanner.text = "HTTP Error " + xhr.status;
                            infoBanner.show();
                            okButton.enabled = true;
                        }
                    }
                }
        xhr.send("email="+user+"&password="+passw);
    }

    function loaded(jsonObject) {
        console.log(jsonObject.data.api_token);
        loader.setApiKey(jsonObject.data.api_token);
        loader.setUserName(jsonObject.data.fullname);
    }
}

