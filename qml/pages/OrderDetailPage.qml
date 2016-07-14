// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtGraphicalEffects 1.0
import org.ekkescorner.data 1.0

import "../common"
import "../navigation"

Page {
    id: orderDetailPage
    focus: true
    property string name: "OrderDetailPage"

    property Customer customer
    property Order order
    property SettingsData settingsData
    property int orderNr: -2
    onOrderNrChanged: {
        if(orderNr > 0) {
            order = dataManager.findOrderByNr(orderNr)
            // already resolved for the list
            // dataManager.resolveOrderReferences(order)
            customer = order.customerAsDataObject
        } else {
            order = dataManager.createOrder()
            var today = new Date()
            order.orderDate = today
            customer = dataManager.customerPropertyList[0]
        }
    }
    property bool isModified: order.expressDelivery != expressSwitch.checked || order.remarks != remarksTextField.text || order.nr <= 0

    footer: FooterCancelSave {
        id: footerButtons
        visible: isModified
        onButtonClickedChanged: {
            if(buttonClicked == footerButtons.buttonRESET) {
                return
            }
            if( buttonClicked == footerButtons.buttonSAVE) {
                // Important: without this the Textfield doesn't free if hardware keyboard connected
                remarksTextField.focus = false
                remarksTextField.text = remarksTextField.text.trim()
                if(remarksTextField.text.length == 0) {
                    footerButtons.reset()
                    appWindow.showError(qsTr("Entry missing:\nPlease enter something into the\n'Remarks' field"))
                    return
                }
                order.expressDelivery = expressSwitch.checked
                order.remarks = remarksTextField.text
                if(order.nr <= 0) {
                    // NEW Order
                    // get next number
                    settingsData = dataManager.settingsData()
                    settingsData.lastUsedNumber = settingsData.lastUsedNumber+1
                    order.nr = settingsData.lastUsedNumber
                    // resolve the customer reference
                    order.resolveCustomerAsDataObject(customer)
                    // insert into list of active Orders (important to get them cachehd)
                    dataManager.insertOrder(order)
                    orderNr = order.nr
                }
            }
            if(buttonClicked == footerButtons.buttonCANCEL) {
                // Important: without this the Textfield doesn't free if hardware keyboard connected
                remarksTextField.focus = false
                if(order.nr > 0) {
                    expressSwitch.checked = order.expressDelivery
                    remarksTextField.text = order.remarks
                } else {
                    // NEW canceled
                    dataManager.undoCreateOrder(order)
                    // GO back
                    navPane.popOnePage()
                }
            }
            footerButtons.reset()
            // want to go back to list now ?
            // navPane.popOnePage()
        }
    }


    Flickable {
        id: flickable
        property string name: "orderDetail"

        contentHeight: root.implicitHeight
        anchors.fill: parent

        Pane {
            id: root
            anchors.fill: parent
            ColumnLayout {
                anchors.right: parent.right
                anchors.left: parent.left
                LabelHeadline {
                    leftPadding: 10
                    text: qsTr("Order Data (QObject*)")
                }
                RowLayout {
                    Layout.leftMargin: 10
                    LabelSubheading {
                        text: qsTr("Customer")
                        color: primaryColor
                        Layout.preferredWidth: 3
                    }
                    Rectangle {
                        id: abcRectangle
                        property var myColors: ["green","grey","red"]
                        property var myText: ["A","B","C"]
                        Layout.preferredWidth: 1
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        anchors.right: parent.right
                        color: myColors[customer.abc]
                        LabelBody {
                            anchors.centerIn: parent
                            text: abcRectangle.myText[customer.abc]
                            color: "white"
                        }
                    } // abc bar
                }
                RowLayout {
                    Layout.leftMargin: 16
                    LabelBodySecondary {
                        topPadding: 6
                        leftPadding: 10
                        rightPadding: 10
                        wrapMode: Text.WordWrap
                        text: qsTr("Nr")
                        Layout.preferredWidth: 1
                    }
                    LabelBody {
                        topPadding: 6
                        leftPadding: 10
                        rightPadding: 10
                        wrapMode: Text.WordWrap
                        text: customer.nr
                        Layout.preferredWidth: 2
                    }
                } // row
                RowLayout {
                    Layout.leftMargin: 16
                    LabelBodySecondary {
                        topPadding: 6
                        leftPadding: 10
                        rightPadding: 10
                        wrapMode: Text.WordWrap
                        text: qsTr("Name")
                        Layout.preferredWidth: 1
                    }
                    LabelBody {
                        topPadding: 6
                        leftPadding: 10
                        rightPadding: 10
                        wrapMode: Text.WordWrap
                        text: customer.name
                        Layout.preferredWidth: 2
                    }
                } // row
                RowLayout {
                    Layout.leftMargin: 16
                    LabelBodySecondary {
                        topPadding: 6
                        leftPadding: 10
                        rightPadding: 10
                        wrapMode: Text.WordWrap
                        text: qsTr("Street")
                        Layout.preferredWidth: 1
                    }
                    LabelBody {
                        topPadding: 6
                        leftPadding: 10
                        rightPadding: 10
                        wrapMode: Text.WordWrap
                        text: customer.street
                        Layout.preferredWidth: 2
                    }
                } // row
                RowLayout {
                    Layout.leftMargin: 16
                    LabelBodySecondary {
                        topPadding: 6
                        leftPadding: 10
                        rightPadding: 10
                        wrapMode: Text.WordWrap
                        text: qsTr("Zip, City")
                        Layout.preferredWidth: 1
                    }
                    LabelBody {
                        topPadding: 6
                        leftPadding: 10
                        rightPadding: 10
                        wrapMode: Text.WordWrap
                        text: customer.zip + " " + customer.city
                        Layout.preferredWidth: 2
                    }
                } // row
                HorizontalDivider {}
                LabelSubheading {
                    leftPadding: 10
                    text: qsTr("Order")
                    color: primaryColor
                }
                RowLayout {
                    Layout.leftMargin: 16
                    LabelBodySecondary {
                        topPadding: 6
                        leftPadding: 10
                        rightPadding: 10
                        wrapMode: Text.WordWrap
                        text: qsTr("Nr")
                        Layout.preferredWidth: 1
                    }
                    LabelBody {
                        topPadding: 6
                        leftPadding: 10
                        rightPadding: 10
                        wrapMode: Text.WordWrap
                        text: order.nr <= 0? qsTr("** NEW **") : order.nr
                        color: order.nr <= 0? Material.color(Material.Red, Material.Shade500) : Material.foreground
                        font.bold: order.nr <= 0? true : false
                        Layout.preferredWidth: 2
                    }
                } // row
                RowLayout {
                    Layout.leftMargin: 16
                    LabelBodySecondary {
                        topPadding: 6
                        leftPadding: 10
                        rightPadding: 10
                        wrapMode: Text.WordWrap
                        text: qsTr("Order Date")
                        Layout.preferredWidth: 1
                    }
                    LabelBody {
                        topPadding: 6
                        leftPadding: 10
                        rightPadding: 10
                        wrapMode: Text.WordWrap
                        text: Qt.formatDate(order.orderDate, Qt.DefaultLocaleLongDate)
                        Layout.preferredWidth: 2
                    }
                } // row
                RowLayout {
                    Layout.leftMargin: 16
                    Layout.rightMargin: 16
                    LabelBodySecondary {
                        id: remarksLabel
                        leftPadding: 10
                        rightPadding: 10
                        wrapMode: Text.WordWrap
                        text: qsTr("Remarks")
                        Layout.preferredWidth: 1
                    }
                    Pane {
                        topPadding: 6
                        leftPadding: 10
                        rightPadding: 10
                        Layout.fillWidth: true
                        Layout.preferredWidth: 2
                        TextField {
                            id: remarksTextField
                            focus: true
                            anchors.fill: parent
                            topPadding: 6
                            leftPadding: 6
                            //rightPadding: 10
                            wrapMode: Text.WordWrap
                            placeholderText: qsTr("Remarks, Hints, Description")
                            text: order.remarks
                            // TODO feature request textChanging and textChanged - per ex trim
                            //Layout.fillWidth: true
                            //Layout.preferredWidth: 2
                        } // remarksTextField
                    } // text pane

                } // row
                RowLayout {
                    Layout.leftMargin: 16
                    LabelBodySecondary {
                        anchors.verticalCenter: parent.verticalCenter
                        leftPadding: 10
                        rightPadding: 10
                        wrapMode: Text.WordWrap
                        text: qsTr("Express Delivery")
                        Layout.preferredWidth: 1
                    }
                    Item {
                        Layout.fillWidth: true
                        Layout.preferredWidth: 2
                        Layout.preferredHeight: expressSwitch.implicitHeight
                        Switch {
                            id: expressSwitch
                            anchors.left: parent.left
                            anchors.verticalCenter: parent.verticalCenter
                            leftPadding: 10
                            rightPadding: 10
                            checked: order.expressDelivery
                        } // switch
                    } // switch item
                } // row
                LabelBodySecondary {
                    visible: !isModified
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("Modify Remarks or Express Delivery: CANCEL / SAVE Buttons become visible")
                    font.italic: true
                }
                HorizontalDivider {}
                LabelSubheading {
                    leftPadding: 10
                    text: qsTr("Positions")
                    color: primaryColor
                }
                LabelBodySecondary {
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("Here you could place a ListView with Positions or create Positions using Repeater or use FAB to push List of Positions on top")
                    font.italic: true
                }
            } // col layout
        } // root
        ScrollIndicator.vertical: ScrollIndicator { }
    } // flickable
    // emitting a Signal could be another option
    Component.onDestruction: {
        cleanup()
    }

    // called immediately after Loader.loaded
    function init() {
        console.log(qsTr("Init done from OrderDetailPage"))
        remarksTextField.forceActiveFocus()
        console.log("REMARKS FOCUS ?"+remarksTextField.activeFocus)
    }
    // called from Component.destruction
    function cleanup() {
        console.log(qsTr("Cleanup done from OrderDetailPage"))
    }

} // carPage
