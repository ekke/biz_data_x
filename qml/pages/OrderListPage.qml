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

    // HEADER
    Component {
        id: headerComponent
        // placed the header controls inside a ToolBar
        // otherwise the header whill shine through to list content
        // while scrolling with ListView.OverlayHeader
        ToolBar {
            width: parent.width
            // default stackorder of 1 doesn't work
            z:2
            // here we set the background to list background
            background: Rectangle{color: Material.background}
            // now header controls work as expected
            ColumnLayout {
                width: parent.width
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
                        opacity: secondaryTextOpacity
                        wrapMode: Label.WordWrap
                        font.italic: true
                        font.bold: true
                    } // label order
                    LabelSubheading {
                        Layout.fillWidth: false
                        leftPadding: 12
                        rightPadding: 24
                        text: qsTr("Express Delivery")
                        opacity: secondaryTextOpacity
                        wrapMode: Label.WordWrap
                        font.italic: true
                        font.bold: true
                    } // label express
                } // end Row Layout
                HorizontalListDivider{}
            } // end Col Layout
        } // toolbar
    } // header component
    // SECTION HEADER
    Component {
        id: sectionHeading
        ToolBar {
            width: parent.width
            // using z 1 because section header must under list header
            z:1
            background: Rectangle{color: Material.background}
            ColumnLayout {
                width: parent.width
                LabelSubheading {
                    topPadding: 6
                    bottomPadding: 6
                    leftPadding: 24
                    text: Qt.formatDate(section, Qt.DefaultLocaleLongDate)
                    anchors.verticalCenter: parent.verticalCenter
                    color: primaryColor
                }
                HorizontalListDivider{}
            } // col layout
        } // toolbar
    }

    // LIST ROW
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

    // LIST VIEW
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
        // in Landscape header scrolls away
        // in protrait header always visible
        headerPositioning: isLandscape? ListView.PullBackHeader : ListView.OverlayHeader

        section.property: "orderDate"
        section.criteria: ViewSection.FullString
        section.delegate: sectionHeading

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
