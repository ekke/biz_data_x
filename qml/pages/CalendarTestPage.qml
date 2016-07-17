// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtGraphicalEffects 1.0
import org.ekkescorner.data 1.0

import "../common"
import "../popups"

Flickable {
    id: flickable
    // index to get access to Loader (Destination)
    property int myIndex: index
    contentHeight: Math.max(parent.height, root.implicitHeight)
    // StackView manages this, so please no anchors here
    property string name: "CalendarTestPage"

    Pane {
        id: root
        property date myDate: new Date()
        property string myTime: Qt.formatTime(new Date(),"HH:mm")
        property bool onlyQuartersAllowed: false
        property bool useWorkTimes: false
        anchors.fill: parent
        ColumnLayout {
            anchors.right: parent.right
            anchors.left: parent.left
            Layout.fillWidth: true
            LabelHeadline {
                topPadding: 6
                leftPadding: 10
                text: qsTr("Test Date and Time Picker")
            }
            HorizontalDivider {}
            LabelTitle {
                id: selectedDateLabel
                topPadding: 40
                leftPadding: 30
                rightPadding: 10
                text: Qt.formatDate(root.myDate, Qt.DefaultLocaleLongDate)
                color: primaryColor
            }
            LabelTitle {
                id: selectedTimeLabel
                topPadding: 40
                leftPadding: 30
                rightPadding: 10
                text: root.myTime
                color: primaryColor
            }
            Switch {
                id:qSwitch
                leftPadding: 30
                text: qsTr("every 15 Minutes")
                checked: root.onlyQuartersAllowed
                onCheckedChanged: {
                    root.onlyQuartersAllowed = checked
                }
            }
            Switch {
                id:wSwitch
                leftPadding: 30
                text: qsTr("use Work Hours")
                checked: root.useWorkTimes
                onCheckedChanged: {
                    root.useWorkTimes = checked
                }
            }

            //


        } // col layout
    } // root
    ScrollIndicator.vertical: ScrollIndicator { }

    ButtonFlat {
        z: 1
        bottomPadding: 24
        text: "Date Picker"
        textColor: accentColor
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.leftMargin: 30
        onClicked: {
            datePicker.selectedDate = root.myDate
            datePicker.displayMonth = root.myDate.getMonth()
            datePicker.displayYear = root.myDate.getFullYear()
            datePicker.open()
        }
    }
    ButtonFlat {
        z: 1
        bottomPadding: 24
        text: "Time Picker"
        textColor: accentColor
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.rightMargin: 30
        onClicked: {
            timePicker.open()
            timePicker.setDisplay(root.myTime, root.onlyQuartersAllowed, root.useWorkTimes)
        }
    }

    DatePicker {
        id: datePicker
        onClosed: {
            if(datePicker.isOK) {
                root.myDate = datePicker.selectedDate
            }
        }
    }

    TimePicker {
        id: timePicker
        onClosed: {
            if(timePicker.isOK) {
                root.myTime = timePicker.hrsDisplay+":"+timePicker.minutesDisplay
                root.useWorkTimes = timePicker.useWorkTimes
                root.onlyQuartersAllowed = timePicker.onlyQuartersAllowed
            }
        }
    }

    // emitting a Signal could be another option
    Component.onDestruction: {
        cleanup()
    }

    // called immediately after Loader.loaded
    function init() {
        console.log(qsTr("Init done from Calendar test page"))
    }
    // called from Component.destruction
    function cleanup() {
        console.log(qsTr("Cleanup done from Calendar test page"))
    }
} // flickable
