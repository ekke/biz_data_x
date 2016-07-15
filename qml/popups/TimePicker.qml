// ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.0
import Qt.labs.calendar 1.0
import "../common"

Popup {
    id: timePicker
    property bool isLandscape: parent.width > parent.height
    property int timePickerWidth: parent.width * 0.80
    property int timePickerHeight: parent.height * 0.80
    x: (parent.width - timePickerWidth) / 2
    y: (parent.height - timePickerHeight) / 2
    implicitWidth: timePickerWidth
    implicitHeight: timePickerHeight

    property int timeButtonsPaneSize: isLandscape? Math.min(timePicker.implicitHeight, timePicker.implicitWidth) - 40 - 20 : Math.min(timePicker.implicitHeight, timePicker.implicitWidth) - 40
    property int innerButtonsPaneSize: timeButtonsPaneSize - 80

    // c1: circle 1 (outside) 1-12
    // c2: circle 2 (inside) 13-00
    // d: work day (outside) 7-18
    // n: night (inside) 19-6
    // m: minutes
    // q: only quarters allowed for minutes, disable the other ones
    // data model used to display labels on picker circles
    property var timePickerModel: [
        {"c1":"12","c2":"00","d":"12","n":"00","m":"00","q":true},
        {"c1":"1","c2":"13","d":"13","n":"1","m":"05","q":false},
        {"c1":"2","c2":"14","d":"14","n":"2","m":"10","q":false},
        {"c1":"3","c2":"15","d":"15","n":"3","m":"15","q":true},
        {"c1":"4","c2":"16","d":"16","n":"4","m":"20","q":false},
        {"c1":"5","c2":"17","d":"17","n":"5","m":"25","q":false},
        {"c1":"6","c2":"18","d":"18","n":"6","m":"30","q":true},
        {"c1":"7","c2":"19","d":"7","n":"19","m":"35","q":false},
        {"c1":"8","c2":"20","d":"8","n":"20","m":"40","q":false},
        {"c1":"9","c2":"21","d":"9","n":"21","m":"45","q":true},
        {"c1":"10","c2":"22","d":"10","n":"22","m":"50","q":false},
        {"c1":"11","c2":"23","d":"11","n":"23","m":"55","q":false}
    ]
    // this model used to display selected time
    // so you can add per ex. AM, PM or so
    property var timePickerDisplayModel: [
        {"c1":"12","c2":"00","d":"12","n":"00","m":"00","q":true},
        {"c1":"01","c2":"13","d":"13","n":"01","m":"05","q":false},
        {"c1":"02","c2":"14","d":"14","n":"02","m":"10","q":false},
        {"c1":"03","c2":"15","d":"15","n":"03","m":"15","q":true},
        {"c1":"04","c2":"16","d":"16","n":"04","m":"20","q":false},
        {"c1":"05","c2":"17","d":"17","n":"05","m":"25","q":false},
        {"c1":"06","c2":"18","d":"18","n":"06","m":"30","q":true},
        {"c1":"07","c2":"19","d":"07","n":"19","m":"35","q":false},
        {"c1":"08","c2":"20","d":"08","n":"20","m":"40","q":false},
        {"c1":"09","c2":"21","d":"09","n":"21","m":"45","q":true},
        {"c1":"10","c2":"22","d":"10","n":"22","m":"50","q":false},
        {"c1":"11","c2":"23","d":"11","n":"23","m":"55","q":false}
    ]

    // set these properties before start
    property int outerButtonIndex: 0
    property int innerButtonIndex: -1
    property bool pickMinutes: false
    property bool useWorkTimes: true
    property bool onlyQuartersAllowed: true

    property string hrsDisplay: "12"
    property string minutesDisplay: "00"
    function showHour(hour) {
        for(var i=0; i < timePickerDisplayModel.length; i++) {
            var h = timePickerDisplayModel[i]
            if(useWorkTimes) {
                if(h.d == hour) {
                    pickMinutes = false
                    innerButtonIndex = -1
                    outerButtonIndex = i
                    return
                }
                if(h.n == hour) {
                    pickMinutes = false
                    outerButtonIndex = -1
                    innerButtonIndex = i
                    return
                }
            } else {
                if(useWorkTimes) {
                    if(h.c1 == hour) {
                        pickMinutes = false
                        innerButtonIndex = -1
                        outerButtonIndex = i
                        return
                    }
                    if(h.c2 == hour) {
                        pickMinutes = false
                        outerButtonIndex = -1
                        innerButtonIndex = i
                        return
                    }
                }
            }
        }
    }
    function showMinutes(minutes) {
        for(var i=0; i < timePickerDisplayModel.length; i++) {
            var m = timePickerDisplayModel[i]
            if(m.m == minutes) {
                innerButtonIndex = -1
                outerButtonIndex = i
                pickMinutes = true
                return
            }
        }
    } // showMinutes
    function checkDisplay() {
        if(pickMinutes) {
            hrsButton.checked = false
            minutesButton.checked = true
        } else {
            minutesButton.checked = false
            hrsButton.checked = true
        }
    }

    topPadding: 0
    leftPadding: 0
    rightPadding: 0

    Pane {
        id: headerPane
        padding: 0

        implicitWidth: timePicker.isLandscape? parent.width - timePicker.timeButtonsPaneSize - 40 : parent.width
        implicitHeight:timePicker.isLandscape? timePicker.timePickerHeight : timePicker.timePickerHeight - timePicker.timeButtonsPaneSize - 40 - 20
        background: Rectangle {
            color: Material.primary
        }

        GridLayout {
            id: headerGrid
            property int myPointSize: timePicker.isLandscape? 48 : 64
            anchors.centerIn: parent
            rows: timePicker.isLandscape? 4 : 2
            columns: timePicker.isLandscape? 1 : 3
            rowSpacing: 0

            Label {
                id: titleLabel
                Layout.columnSpan: timePicker.isLandscape? 1 : 3
                text: qsTr("Time (HH:MM)")
                color: "white"
                Layout.alignment: Text.AlignHCenter
                font.pointSize: 36
                fontSizeMode: Text.Fit
            }

            Button {
                id: hrsButton
                Layout.alignment: timePicker.isLandscape? Text.AlignHCenter : Text.AlignRight
                checked: true
                checkable: true
                contentItem: Label {
                    text: timePicker.hrsDisplay
                    font.pointSize: headerGrid.myPointSize
                    fontSizeMode: Text.Fit
                    opacity: hrsButton.checked ? 1.0 : 0.6
                    color: "white"
                    elide: Text.ElideRight
                }
                background: Rectangle {
                    color: "transparent"
                }
                onClicked: {
                    minutesButton.checked = false
                    hrsButton.checked = true
                    timePicker.pickMinutes = false
                    timePicker.showHour(timePicker.hrsDisplay)
                }
            } // hrsButton

            Label {
                text: ":"
                Layout.alignment: Text.AlignHCenter
                font.pointSize: headerGrid.myPointSize
                fontSizeMode: Text.Fit
                opacity: 0.6
                color: "white"
            }

            Button {
                id: minutesButton
                Layout.alignment: timePicker.isLandscape? Text.AlignHCenter : Text.AlignLeft
                checked: false
                checkable: true
                contentItem: Label {
                    text: timePicker.minutesDisplay
                    font.pointSize: headerGrid.myPointSize
                    fontSizeMode: Text.Fit
                    opacity: minutesButton.checked ? 1.0 : 0.6
                    color: "white"
                    elide: Text.ElideRight
                }
                background: Rectangle {
                    color: "transparent"
                }
                onClicked: {
                    hrsButton.checked = false
                    minutesButton.checked = true
                    timePicker.pickMinutes = true
                    timePicker.showMinutes(timePicker.minutesDisplay)
                }
            } // hrsButton
        } // header grid


    } // headerPane

    Pane {
        id: footerPane
        padding: 0
        anchors.right: parent.right
        y: timePicker.isLandscape? parent.height - 20 - 20 : parent.height - 40

        implicitWidth: timePicker.timeButtonsPaneSize + 40
        implicitHeight: 40
        background: Rectangle {
            color: "transparent"
        }
        ColumnLayout {
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top: parent.top

            RowLayout {
                Layout.fillHeight: true
                ButtonFlat {
                    Layout.fillWidth: true
                    Layout.preferredWidth: 1
                    text: qsTr("Cancel")
                    textColor: Material.primary
                    onClicked: {
                        timePicker.close()
                    }
                } // cancel button
                Item {
                    Layout.fillWidth: true
                    Layout.preferredWidth: 1
                }
                ButtonFlat {
                    Layout.fillWidth: true
                    Layout.preferredWidth: 1
                    text: qsTr("OK")
                    textColor: Material.primary
                    onClicked: {
                        // root.myTime = timePicker.hrsDisplay+":"+timePicker.minutesDisplay
                        timePicker.close()
                    }
                } // ok button
            } // button row
        } // button col

    } // footer pane

    Pane {
        id: timeButtonsPane
        implicitWidth: timePicker.timeButtonsPaneSize
        implicitHeight: timePicker.timeButtonsPaneSize
        padding: 0
        x: timePicker.isLandscape? timePicker.timePickerWidth - timePicker.timeButtonsPaneSize - 20 : 20
        y: timePicker.isLandscape? 24 : timePicker.timePickerHeight - timePicker.timeButtonsPaneSize - 20 - 20
        background: Rectangle {color: "transparent"}

        Rectangle {
            anchors.centerIn: parent
            width: timePicker.timeButtonsPaneSize + 10
            height: timePicker.timeButtonsPaneSize + 10
            color: Material.color(Material.Grey, Material.Shade50)
            radius: width / 2
        }

        ButtonGroup {
            id: outerButtonGroup
        }
        ButtonGroup {
            id: innerButtonGroup
        }

        Pane {
            id: innerButtonsPane
            implicitWidth: timePicker.innerButtonsPaneSize
            implicitHeight: timePicker.innerButtonsPaneSize
            padding: 0
            visible: !timePicker.pickMinutes
            anchors.centerIn: parent
            background: Rectangle {color: "transparent"}

            Repeater {
                id: innerRepeater
                model: timePicker.timePickerModel
                delegate: Button {
                    id: innerButton
                    text: timePicker.useWorkTimes? modelData.n : modelData.c2
                    font.bold: checked
                    x: timePicker.innerButtonsPaneSize / 2 - width / 2 //- 20
                    y: timePicker.innerButtonsPaneSize / 2 - height / 2 //- 20
                    width: 40
                    height: 40
                    checked: index == timePicker.innerButtonIndex
                    checkable: true

                    onClicked: {
                        timePicker.outerButtonIndex = -1
                        timePicker.innerButtonIndex = index
                        if(timePicker.useWorkTimes) {
                            timePicker.hrsDisplay = timePicker.timePickerDisplayModel[index].n
                        } else {
                            timePicker.hrsDisplay = timePicker.timePickerDisplayModel[index].c2
                        }
                    }

                    ButtonGroup.group: innerButtonGroup

                    property real angle: 360 * (index / innerRepeater.count)

                    transform: [
                        Translate {
                            y: -timePicker.innerButtonsPaneSize * 0.5 + innerButton.height / 2
                        },
                        Rotation {
                            angle: innerButton.angle
                            origin.x: innerButton.width / 2
                            origin.y: innerButton.height / 2
                        }
                    ]

                    contentItem: Label {
                        text: innerButton.text
                        font: innerButton.font
                        minimumPointSize: 8
                        fontSizeMode: Text.Fit
                        opacity: innerButton.checked ? 1.0 : enabled || innerButton.highlighted ? 1.0 : 0.6
                        color: innerButton.checked || innerButton.highlighted ? "white" : Material.color(Material.Grey, Material.Shade500)
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                        rotation: -innerButton.angle
                    } // content Label

                    background: Rectangle {
                        color: innerButton.checked ? Material.primary : "transparent"
                        radius: width / 2
                    }
                } // inner button
            } // innerRepeater
        } // innerButtonsPane

        Repeater {
            id: outerRepeater
            model: timePicker.timePickerModel
            delegate: Button {
                id: outerButton
                text: timePicker.pickMinutes? modelData.m : timePicker.useWorkTimes? modelData.d : modelData.c1
                font.bold: checked || timePicker.pickMinutes && timePicker.onlyQuartersAllowed
                x: timePicker.timeButtonsPaneSize / 2 - width / 2
                y: timePicker.timeButtonsPaneSize / 2 - height / 2
                width: 40
                height: 40
                checked: index == timePicker.outerButtonIndex
                checkable: true
                enabled: timePicker.pickMinutes && timePicker.onlyQuartersAllowed? modelData.q : true

                onClicked: {
                    timePicker.innerButtonIndex = -1
                    timePicker.outerButtonIndex = index
                    if(timePicker.pickMinutes) {
                        timePicker.minutesDisplay = timePicker.timePickerDisplayModel[index].m
                    } else {
                        if(timePicker.useWorkTimes) {
                            timePicker.hrsDisplay = timePicker.timePickerDisplayModel[index].d
                        } else {
                            timePicker.hrsDisplay = timePicker.timePickerDisplayModel[index].c1
                        }
                    }
                }

                ButtonGroup.group: outerButtonGroup

                property real angle: 360 * (index / outerRepeater.count)

                transform: [
                    Translate {
                        y: -timePicker.timeButtonsPaneSize * 0.5 + outerButton.height / 2
                    },
                    Rotation {
                        angle: outerButton.angle
                        origin.x: outerButton.width / 2
                        origin.y: outerButton.height / 2
                    }
                ]

                contentItem: Label {
                    text: outerButton.text
                    font: outerButton.font
                    minimumPointSize: 8
                    fontSizeMode: Text.Fit
                    opacity: enabled || outerButton.highlighted || outerButton.checked ? 1 : 0.3
                    color: outerButton.checked || outerButton.highlighted ? "white" : timePicker.pickMinutes && timePicker.onlyQuartersAllowed? Material.primary : Material.foreground
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                    rotation: -outerButton.angle
                } // outer content label

                background: Rectangle {
                    color: outerButton.checked ? Material.primary : "transparent"
                    radius: width / 2
                }
            } // outer button
        } // outerRepeater



        Rectangle {
            // line to outer buttons
            visible: timePicker.outerButtonIndex >= 0
            x: timePicker.timeButtonsPaneSize / 2
            y: timePicker.timeButtonsPaneSize / 2 - height
            width: 1
            height: timePicker.timeButtonsPaneSize / 2 - 40
            transformOrigin: Item.Bottom
            rotation: outerButtonGroup.checkedButton? outerButtonGroup.checkedButton.angle : 0
            color: Material.primary
            antialiasing: true
        } // line to outer buttons

        Rectangle {
            // line to inner buttons
            visible: timePicker.innerButtonIndex >= 0 && !timePicker.pickMinutes
            x: timePicker.timeButtonsPaneSize / 2
            y: timePicker.timeButtonsPaneSize / 2 - height
            width: 1
            height: timePicker.innerButtonsPaneSize / 2 - 40
            transformOrigin: Item.Bottom
            rotation: innerButtonGroup.checkedButton? innerButtonGroup.checkedButton.angle : 0
            color: Material.primary
            antialiasing: true
        } // line to outer buttons

        Rectangle {
            // centerpoint
            anchors.centerIn: parent
            width: 10
            height: 10
            color: Material.primary
            radius: width / 2
        }

    } // timeButtonsPane


} // timePicker


