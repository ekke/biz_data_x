// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtGraphicalEffects 1.0

import "../pages"
import "../common"

Page {
    id: navPage
    property alias depth: navPane.depth
    property string name: "OrderNavPage"
    // index to get access to Loader (Destination)
    property int myIndex: index

    StackView {
        id: navPane
        anchors.fill: parent
        property string name: "OrdersNavPane"
        focus: true

        initialItem: OrdersPage{}

        Loader {
            id: orderListPageLoader
            active: false
            visible: false
            source: "../pages/OrderListPage.qml"
            onLoaded: {
                navPane.push(item)
            }
        }
        Loader {
            id: orderDetailPageLoader
            property int orderNr: -1
            active: false
            visible: false
            source: "../pages/OrderDetailPage.qml"
            onLoaded: {
                item.orderNr = orderNr
                navPane.push(item)
                item.init()
            }
        }

        property var ordersMarkedToDelete: []
        function markOrderToDelete(orderNr) {
            ordersMarkedToDelete.push(orderNr)
        }
        function deleteOrders() {
            if(ordersMarkedToDelete.length == 0) {
                return
            }
            // delete marked orders
            for (var i = 0; i < ordersMarkedToDelete.length; i++) {
                var orderNr = ordersMarkedToDelete[i]
                console.log("remove from Orders nr:"+orderNr)
                dataManager.deleteOrderByNr(orderNr)
            }
            // optional: appWindow.showToast(qsTr("Orders deleted: ")+ordersMarkedToDelete.length)
            ordersMarkedToDelete = []
            console.log("Orders to be deleted: "+ordersMarkedToDelete.length)
        }

        function pushOrderListPage() {
            dataManager.resolveReferencesForAllOrder()
            orderListPageLoader.active = true
        }
        function pushOrderDetail(orderNr) {
            orderDetailPageLoader.orderNr = orderNr
            orderDetailPageLoader.active = true
        }

        function popOnePage() {
            var page = pop()
            if(page.name == "OrderListPage") {
                orderListPageLoader.active = false
                deleteOrders()
                return
            }
            if(page.name == "OrderDetailPage") {
                orderDetailPageLoader.active = false
                return
            }
        } // popOnePage

    } // navPane

    FloatingActionButton {
        visible: navPane.depth == 1 || (navPane.depth == 2 && navPane.get(1).name == "OrderListPage")
        property string imageName: navPane.depth == 2 || dataManager.orderPropertyList.length == 0 ? "/add.png" : "/list.png"
        z: 1
        anchors.margins: 20
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        imageSource: "qrc:/images/"+iconOnPrimaryDarkFolder+imageName
        backgroundColor: primaryDarkColor
        onClicked: {
            if(navPane.depth == 2 || dataManager.orderPropertyList.length == 0) {
                // adding a new order will change the list
                // so at first delete wentries marked to be deleted
                navPane.deleteOrders()
                // push an empty order
                navPane.pushOrderDetail(-1)
            } else {
                navPane.pushOrderListPage()
            }
        }
    } // FAB

    function destinationAboutToChange() {
        navPane.deleteOrders()
    }

    // triggered from BACK KEYs:
    // a) Android system BACK
    // b) Back Button from TitleBar
    function goBack() {
        // check if goBack is allowed
        if(navPane.get(navPane.depth-1).name == "OrderDetailPage") {
            if(navPane.get(navPane.depth-1).isModified) {
                // it's up to you: show Info Toast
                // appWindow.showInfo(qsTr("Please Cancel or Save changes first"))
                // or show a Toast:
                appWindow.showToast(qsTr("Please Cancel or Save changes first"))
                // or do auto-cancel or auto-save
                return
            }
        }
        navPane.popOnePage()
    }

    Component.onDestruction: {
        cleanup()
    }

    function init() {
        console.log("INIT OrderNavPane")
        navPane.initialItem.init()
    }
    function cleanup() {
        console.log("CLEANUP OrderNavPane")
    }

} // navPage
