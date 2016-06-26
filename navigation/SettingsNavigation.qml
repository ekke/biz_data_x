// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtGraphicalEffects 1.0

import "../pages"

StackView {
    id: navPane
    property string name: "SettingsNavPane"
    // index to get access to Loader (Destination)
    property int myIndex: index

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
    function popPage() {
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

    Component.onDestruction: {
        cleanup()
    }

    function init() {
        console.log("INIT HomeNavPane")
    }
    function cleanup() {
        console.log("CLEANUP HomeNavPane")
    }

} // navPane
