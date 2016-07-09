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
    property string name: "OrderDetailPage"

    property Customer customer
    property Order order
    property int orderNr: -2
    onOrderNrChanged: {
        if(orderNr >= 0) {
            order = dataManager.findOrderByNr(orderNr)
            // dataManager.resolveOrderReferences(order)
            customer = order.customerAsDataObject
            checkDate()
        } else {
            order = dataManager.createOrder()
            customer = dataManager.customerPropertyList[0]
            order.customer = customer.nr
            dataManager.resolveOrderReferences(order)
        }
    }
    property bool isModified: order.expressDelivery != expressSwitch.checked || order.remarks != remarksTextField.text

    function checkDate() {
        if(!order.hasOrderDate()) {
            console.log("SET DATE")
            var today = new Date()
            order.orderDate = today
            return
        }

    }

    footer: FooterCancelSave {
        id: footerButtons
        visible: isModified
        onButtonClickedChanged: {
            if(buttonClicked == footerButtons.buttonRESET) {
                return
            }
            if( buttonClicked == footerButtons.buttonSAVE) {
                order.expressDelivery = expressSwitch.checked
                order.remarks = remarksTextField.text
            }
            if(buttonClicked == footerButtons.buttonCANCEL) {
                expressSwitch.checked = order.expressDelivery
                remarksTextField.text = order.remarks
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
                        Layout.preferredWidth: 1
                    }
                    Rectangle {
                        property var myColors: ["green","grey","red"]
                        width: 40
                        Layout.fillHeight: true
                        color: myColors[order.customerAsDataObject.abc]
                    }
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
                        text: order.nr
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
                        leftPadding: 10
                        rightPadding: 10
                        wrapMode: Text.WordWrap
                        text: qsTr("Remarks")
                        Layout.preferredWidth: 1
                    }
                    TextField {
                        id: remarksTextField
                        focus: true
                        topPadding: 6
                        leftPadding: 10
                        rightPadding: 10
                        wrapMode: Text.WordWrap
                        placeholderText: qsTr("Optional: Remarks, Hints, Description")
                        text: order.remarks
                        // inputMethodHints: Qt.ImhFormattedNumbersOnly
                        Layout.fillWidth: true
                        Layout.preferredWidth: 2
                    } // remarksTextField
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
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("Editing Remarks or Express Delivery: CANCEL / SAVE Buttons become visible")
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
                    text: qsTr("Here you could place a ListView with Positions or create Positions using Repeater or have FAB to push List of Positions on top")
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
        remarksTextField.focus = true
        console.log("REMARKS FOCUS ?"+remarksTextField.activeFocus)
    }
    // called from Component.destruction
    function cleanup() {
        console.log(qsTr("Cleanup done from OrderDetailPage"))
    }

} // carPage
