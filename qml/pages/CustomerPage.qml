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
    id: customerPage
    property string name: "CustomerPage"
    // index to get access to Loader (Destination)
    property int myIndex: index

    property Customer customer

    Flickable {
        id: flickable
        property string name: "customer"

        contentHeight: root.implicitHeight
        // StackView manages this, so please no anchors here
        anchors.fill: parent

        Pane {
            id: root
            anchors.fill: parent
            ColumnLayout {
                anchors.right: parent.right
                anchors.left: parent.left
                LabelHeadline {
                    leftPadding: 10
                    text: qsTr("Customer Data (QObject*)")
                }
                RowLayout {
                    IconInactive {
                        imageName: modelData.icon
                        imageSize: 48
                    }
                    LabelSubheading {
                        text: qsTr("Stored Customers: ")+ dataManager.customerPropertyList.length
                    }
                }
                RowLayout {
                    LabelSubheading {
                        topPadding: 6
                        leftPadding: 10
                        rightPadding: 10
                        wrapMode: Text.WordWrap
                        text: qsTr("There's only one Customer in this sample app\n'abc' can be changed and shows Marker at Drawer:\n  A = Green\n  B = Grey\n  C = Red")
                    }
                }
                RowLayout {
                    LabelBodySecondary {
                        topPadding: 6
                        leftPadding: 10
                        rightPadding: 10
                        wrapMode: Text.WordWrap
                        text: qsTr("Activation Policy: ")
                    }
                    LabelBody {
                        topPadding: 6
                        leftPadding: 10
                        rightPadding: 10
                        wrapMode: Text.WordWrap
                        text: qsTr("LAZY")
                    }
                }
                HorizontalDivider {}
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
                RowLayout {
                    LabelBodySecondary {
                        topPadding: 6
                        leftPadding: 10
                        rightPadding: 10
                        wrapMode: Text.WordWrap
                        text: qsTr("A B C")
                        Layout.preferredWidth: 3
                    }
                    RadioButton {
                        leftPadding: 10
                        Layout.preferredWidth: 2
                        Layout.fillWidth: true
                        checked: customer.abc == 0
                        text: qsTr("A")
                        onCheckedChanged: {
                            if(checked) {
                                customer.abc = 0
                                rootPane.updateCustomerMarker(customer.abc)
                            }
                        }
                    }
                    RadioButton {
                        Layout.preferredWidth: 2
                        Layout.fillWidth: true
                        checked: customer.abc == 1
                        text: qsTr("B")
                        onCheckedChanged: {
                            if(checked) {
                                customer.abc = 1
                                rootPane.updateCustomerMarker(customer.abc)
                            }
                        }
                    }
                    RadioButton {
                        rightPadding: 10
                        Layout.preferredWidth: 2
                        Layout.fillWidth: true
                        checked: customer.abc == 2
                        text: qsTr("C")
                        onCheckedChanged: {
                            if(checked) {
                                customer.abc = 2
                                rootPane.updateCustomerMarker(customer.abc)
                            }
                        }
                    }
                }
                RowLayout {
                    Item {
                        Layout.preferredWidth: 1
                        Layout.fillWidth: true
                    }
                    Rectangle {
                        id: abcRectangle
                        property var myColors: ["green","grey","red"]
                        property var myText: ["A","B","C"]
                        height: 20
                        Layout.fillWidth: true
                        color: myColors[customer.abc]
                        Layout.preferredWidth: 2
                        LabelBody {
                            anchors.centerIn: parent
                            text: abcRectangle.myText[customer.abc]
                            color: "white"
                        }
                    }
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
        console.log("Init done from CustomerPage")
        customer = dataManager.customerPropertyList[0]
        rootPane.updateCustomerMarker(customer.abc)
    }
    // called from Component.destruction
    function cleanup() {
        console.log("Cleanup done from CustomerPage")
    }

} // carPage
