// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtGraphicalEffects 1.0
import org.ekkescorner.data 1.0
import "../common"

Page {
    id: orderListPage
    property string name: "OrderListPage"
    bottomPadding: 24
    topPadding: 16

    Component {
        id: headerComponent
        ColumnLayout {
            width: parent.width
            implicitHeight: 40
            RowLayout {
                spacing: 20
                Layout.fillWidth: true
                LabelSubheading {
                    leftPadding: 24
                    rightPadding: 12
                    Layout.preferredWidth: 1
                    Layout.fillWidth: true
                    anchors.verticalCenter: parent.verticalCenter
                    text: qsTr("Order Info")
                    color: primaryColor
                    wrapMode: Label.WordWrap
                } // label order
                LabelSubheading {
                    Layout.fillWidth: false
                    leftPadding: 12
                    rightPadding: 24
                    text: qsTr("Express Delivery")
                    color: primaryColor
                    wrapMode: Label.WordWrap
                } // label express
            } // end Row Layout
            HorizontalListDivider{}
        } // end Col Layout
    }

    Component {
        id: orderRowComponent
        ColumnLayout {
            width: parent.width
            implicitHeight: 40
            RowLayout {
                spacing: 20
                Layout.fillWidth: true
                LabelBody {
                    leftPadding: 24
                    rightPadding: 12
                    Layout.preferredWidth: 1
                    Layout.fillWidth: true
                    anchors.verticalCenter: parent.verticalCenter
                    text: model.modelData.remarks + "\n#: " + model.modelData.nr
                    wrapMode: Label.WordWrap
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            listView.currentIndex = index
                            navPane.pushOrderDetail(index)
                        }
                    }
                } // label
                SwitchWithLeftLabel {
                    leftPadding: 12
                    rightPadding: 12
                    text: qsTr("Express")
                    checked: model.modelData.expressDelivery
                    onCheckedChanged: {
                        model.modelData.expressDelivery = checked
                    }
                } // switch express
            } // end Row Layout
            HorizontalListDivider{}
        } // end Col Layout
    } // orderRowComponent

    ListView {
        id: listView
        focus: true
        clip: true
        highlight: Rectangle {color: Material.listHighlightColor }
        currentIndex: -1
        anchors.fill: parent
        // setting the margin to be able to scroll the list above the FAB to use the Switch on last row
        bottomMargin: 40
        // QList<Order*>
        model: dataManager.orderPropertyList

        delegate: orderRowComponent
        header: headerComponent
        headerPositioning: ListView.PullBackHeader


        ScrollIndicator.vertical: ScrollIndicator { }
    } // end listView

    Component.onDestruction: {
        cleanup()
    }

    // called immediately after Loader.loaded
    function init() {
        console.log(qsTr("Init done from OrderListPage"))
    }
    // called from Component.destruction
    function cleanup() {
        console.log(qsTr("Cleanup done from OrderListPage"))
    }
} // end primaryPage
