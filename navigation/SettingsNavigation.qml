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
                item.text = qsTr("Biz Data Settings")
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
                item.text = qsTr("Biz Data Settings")
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
        property string name: "SettingsNavPane"
        focus: true

        initialItem: SettingsPage{}

        Loader {
            id: primaryColorPageLoader
            active: false
            visible: false
            source: "../pages/PrimaryColorPage.qml"
            onLoaded: {
                navPane.push(item)
            }
        }
        Loader {
            id: accentColorPageLoader
            active: false
            visible: false
            source: "../pages/AccentColorPage.qml"
            onLoaded: {
                navPane.push(item)
            }
        }

        function pushPrimaryColorPage() {
            primaryColorPageLoader.active = true
        }
        function pushAccentColorPage() {
            accentColorPageLoader.active = true
        }
        function popOnePage() {
            var page = pop()
            if(page.name == "PrimaryColorPage") {
                primaryColorPageLoader.active = false
                return
            }
            if(page.name == "AccentColorPage") {
                accentColorPageLoader.active = false
                return
            }
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
        console.log("INIT SettingsNavPane")
    }
    function cleanup() {
        console.log("CLEANUP SettingsNavPane")
    }

} // navPage
