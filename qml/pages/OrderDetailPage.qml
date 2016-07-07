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
    property int orderIndex: -2
    onOrderIndexChanged: {
        if(orderIndex >= 0) {
            order = dataManager.orderPropertyList[orderIndex]
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



    footer: ColumnLayout {
        visible: isModified
        z: 2
        anchors.right: parent.right
        anchors.left: parent.left
        RowLayout {
            spacing: 20
            Item {
                Layout.preferredWidth: 1
                Layout.fillWidth: true
            }

            ButtonFlat {
                Layout.preferredWidth: 1
                text: qsTr("Cancel")
                textColor: accentColor
                onClicked: {
                    navPane.popOnePage()
                }
            }
            ButtonFlat {
                Layout.preferredWidth: 1
                text: qsTr("Save")
                textColor: primaryColor
                onClicked: {
                    order.expressDelivery = expressSwitch.checked
                    order.remarks = remarksTextField.text
                    navPane.popOnePage()
                }
            }
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
                LabelSubheading {
                    leftPadding: 10
                    text: qsTr("Customer")
                    color: primaryColor
                }
                RowLayout {
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
                }
                RowLayout {
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
                }
                RowLayout {
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
                }
                RowLayout {
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
                }
                HorizontalDivider {}
                LabelSubheading {
                    leftPadding: 10
                    text: qsTr("Order")
                    color: primaryColor
                }
                RowLayout {
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
                }
                RowLayout {
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
                }
                RowLayout {
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
                        onTextChanged: {

                        }
                        Layout.fillWidth: true
                        Layout.preferredWidth: 2
                    }
                }
                RowLayout {
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
                            onCheckedChanged: {

                            }

                        }
                    }
                }
                LabelBodySecondary {
                    leftPadding: 10
                    rightPadding: 10
                    wrapMode: Text.WordWrap
                    text: qsTr("Change remarks or Express Delivery to get CANCEL / SAVE Buttons visible")
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
