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

    LabelHeadline {
        id: headline
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        leftPadding: 16
        rightPadding: 16
        bottomPadding: 16
        text: qsTr("Select Material Primary Color")
    }

    ListView {
        id: listView
        clip: true
        currentIndex: -1
        anchors.top: headline.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        model: dataManager.orderPropertyList

        delegate: ColumnLayout {
            id: itemDelegate
            // property Order order: model.modelData
            width: parent.width
            implicitHeight: 40
            RowLayout {
                spacing: 10
                LabelBody {
                    id: theLabel
                    Layout.preferredWidth: 1
                    leftPadding: 40
                    anchors.verticalCenter: parent.verticalCenter
                    text: model.modelData.remarks // or only remarks // order.remarks
                    wrapMode: Label.WordWrap
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            navPane.popOnePage()
                        }
                    } // mouse
                }
                Switch {
                    id: theSwitch
                    Layout.preferredWidth: 1
                    Layout.fillWidth: true
                    text: "test"
                    onCheckedChanged: {
                        remarks += checked
                    }
                }

            } // end Row
        } // end delegateItem

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
