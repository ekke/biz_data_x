import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import "../common"

Popup {
    id: popup
    closePolicy: Popup.NoAutoClose
    bottomMargin: isLandscape? 24 : 80
    x: (appWindow.width - width) / 2
    y: (appWindow.height - height)
    background: Rectangle{
        color: Material.color(Material.Red, isDarkTheme? Material.Shade500 : Material.Shade800)
        radius: 24
        opacity: toastOpacity
    }
    Timer {
        id: errorTimer
        interval: 3000
        repeat: false
        onTriggered: {
            popup.close()
        }
    } // toastTimer
    Label {
        id: errorLabel
        leftPadding: 16
        rightPadding: 16
        font.pixelSize: 16
        color: "white"
    } // toastLabel
    onAboutToShow: {
        errorTimer.start()
    }
    function start(errorText) {
        errorLabel.text = errorText
        if(!errorTimer.running) {
            open()
        } else {
            errorTimer.restart()
        }
    } // function start
} // popup errorPopup
