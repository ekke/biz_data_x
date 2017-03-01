// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1
import QtGraphicalEffects 1.0

import "../pages"
import "../common"

Page {
    id: navPage
    property alias depth: navPane.depth
    property string name: "CalendarNavPage"
    // index to get access to Loader (Destination)
    property int myIndex: index

    StackView {
        id: navPane
        anchors.fill: parent
        property string name: "CalendarNavPane"
        focus: true

        initialItem: CalendarTestPage{}

        Loader {
            id: calendarDayPageLoader
            active: false
            visible: false
            source: "../pages/CalendarDayPage.qml"
            onLoaded: {
                navPane.push(item)
                item.init()
            }
        }

        function pushCalendarDay() {
            // calendarDayPageLoader.orderNr = orderNr
            calendarDayPageLoader.active = true
        }

        function popOnePage() {
            var page = pop()
            if(page.name == "CalendarDayPage") {
                calendarDayPageLoader.active = false
                return
            }
        } // popOnePage

    } // navPane


    function destinationAboutToChange() {
        //
    }

    // triggered from BACK KEYs:
    // a) Android system BACK
    // b) Back Button from TitleBar
    function goBack() {
        // check if goBack is allowed
        //
        navPane.popOnePage()
    }

    Component.onDestruction: {
        cleanup()
    }

    function init() {
        console.log("INIT CalendarNavPane")
    }
    function cleanup() {
        console.log("CLEANUP CalendarNavPane")
    }

} // navPage
