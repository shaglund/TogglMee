import QtQuick 1.1
import com.nokia.meego 1.0

PageStackWindow {
    property bool showMain: loader.hasApiKey
    id: appWindow

    Connections {
        target: loader
        onApiKeyChanged: {
            console.log("onApiKeyChanged(" + avail + ")");
            showPage();
        }
    }

    Component.onCompleted: {
        showPage();
    }

    function showPage() {
        if(showMain) {
            mainPageLoader.source = "MainPage.qml";
        } else {
            mainPageLoader.source = "InputApi.qml";
        }
    }

    Loader {
        id: mainPageLoader
        anchors.fill: parent        
        onLoaded: {
            item.visible = true;
            item.parent = appWindow
            if(loader.hasApiKey)
                item.entryModel = timeentryModel;
        }
    }
}

/* TEST..
PageStackWindow {
    initialPage: testPage
    TestPage {
        id: testPage
    }
}
*/
