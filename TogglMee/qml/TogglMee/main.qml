import QtQuick 1.1
import com.nokia.meego 1.0

PageStackWindow {
    id: appWindow
    Loader {
        id: pageLoader
        anchors.fill: parent
        source: loader.hasApiKey ? "MainPage.qml" : ""
        onLoaded: {
            pageStack.push(pageLoader.item)
        }
    }

    InputApi {
        id: inputApi
        visible: !loader.hasApiKey
    }
}
