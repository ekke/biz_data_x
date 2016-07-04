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
    property string name: "HomeNavPage"
    // index to get access to Loader (Destination)
    property int myIndex: index

    StackView {
        id: navPane
        anchors.fill: parent
        property string name: "OrdersNavPane"
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
