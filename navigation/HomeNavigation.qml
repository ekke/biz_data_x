// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtGraphicalEffects 1.0

import "../pages"

Page {
    id: navPage
    property alias depth: navPane.depth
    property string name: "SettingsNavPage"
    // index to get access to Loader (Destination)
    property int myIndex: index

    header: isLandscape ? null : titleBar
    Loader {
        id: titleBar
        visible: !isLandscape
        active: !isLandscape
        source: "DrawerTitleBar.qml"
        onLoaded: {
            if(item) {
                item.text = qsTr("Biz Data Homepage")
            }
        }
    }
    // in LANDSCAPE header is null and we have a floating TitleBar
    Loader {
        id: titleBarFloating
        visible: isLandscape
        anchors.top: parent.top
        anchors.left: parent.left
        // anchors.leftMargin: sideBar.width+6
        anchors.right: parent.right
        active: isLandscape
        source: "DrawerTitleBar.qml"
        onLoaded: {
            if(item) {
                item.text = qsTr("Biz Data Homepage")
            }
        }
    }

    StackView {
        id: navPane
        anchors.top: isLandscape ? titleBarFloating.bottom : parent.top
        anchors.left: parent.left
        anchors.topMargin: isLandscape ? 6 : 0
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        property string name: "HomeNavPane"
        focus: true

        initialItem: HomePage{}

        Loader {
            id: qtPageLoader
            active: true
            visible: false
            source: "../pages/QtPage.qml"
        }

        function pushQtPage() {
            navPane.push(qtPageLoader.item)
        }
        function popOnePage() {
            navPane.pop()
        }


    } // navPane

    // triggered from BACK KEY
    function goBack() {
        navPane.popOnePage()
    }

    Component.onDestruction: {
        cleanup()
    }

    function init() {
        console.log("INIT HomeNavPane")
    }
    function cleanup() {
        console.log("CLEANUP HomeNavPane")
    }

} // navPage
