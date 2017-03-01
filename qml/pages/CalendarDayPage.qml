// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1
import QtGraphicalEffects 1.0
import org.ekkescorner.data 1.0

import "../common"
import "../navigation"

Page {
    id: calendarDayPage
    focus: true
    property string name: "CalendarDayPage"

    header:
        Pane {
        anchors.left: parent.left
        anchors.right: parent.right
        padding:0

        ColumnLayout{
            id: myHeader
            property int offset: 24
            property var dayHeaders: [qsTr("Mo"),qsTr("Tu"),qsTr("We"),qsTr("Th"),qsTr("Fr"),qsTr("Sa"),qsTr("Su")]
            anchors.fill: parent
            Layout.fillWidth: true
            spacing: 0
            Layout.leftMargin: 10
            Layout.rightMargin: 10
            RowLayout {
                Layout.fillWidth: true
                Repeater {
                    model: 8
                    ButtonFlat {
                        textColor: index == 0? "black" : index == 1? accentColor : primaryColor
                        enabled: index != 0
                        text: index == 0? "Week\nxx" : myHeader.dayHeaders[index-1]+"\n"+(index+myHeader.offset)
                    }
                }
            }
            HorizontalDivider{}
        }

    } // pane



    Flickable {
        id: flickable
        property string name: "calendarDay"

        contentHeight: root.implicitHeight
        anchors.fill: parent


        Pane {
            id: root
            anchors.fill: parent
            ColumnLayout {
                //anchors.right: parent.right
                //anchors.left: parent.left
                anchors.fill: parent
                Layout.fillWidth: true
                spacing: 2
                Layout.leftMargin: 10
                Layout.rightMargin: 10

                Repeater {
                    model: 24
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 60
                        LabelBody {
                            Layout.fillWidth: false
                            Layout.fillHeight: true
                            width: 64
                            verticalAlignment: Text.AlignBottom
                            anchors.bottom: parent.bottom
                            text: index < 9 ? ("0"+(index+1) + ":00") : ((index+1) + ":00")
                            font.bold: true
                            transform: Translate { y: 10 }
                        }
                        Pane {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            padding: 0
                            background: Rectangle {
                                anchors.fill: parent
                                color: index == 12 ? primaryDarkColor : index < 9 || index > 16 ? "lightgrey" : Material.color(Material.Teal, Material.Shade100)
                            }
                            Label {
                                leftPadding: 6
                                topPadding: 6
                                anchors.top: parent.top
                                anchors.left: parent.left
                                visible: index == 12
                                text: "Meeting"
                                color: textOnPrimaryDark
                            }
                        }
                    } // row
                } // repeater

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
        console.log("Init done from CalendarDayPage")
    }
    // called from Component.destruction
    function cleanup() {
        console.log("Cleanup done from CalendarDayPage")
    }

    FloatingActionButton {
        //visible: navPane.depth == 1 || (navPane.depth == 2 && navPane.get(1).name == "OrderListPage")
        property string imageName: "/add.png"
        z: 1
        anchors.margins: 20
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        imageSource: "qrc:/images/"+iconOnAccentFolder+imageName
        backgroundColor: accentColor
        onClicked: {
            //
        }
    } // FAB

} // calendarDayPage
