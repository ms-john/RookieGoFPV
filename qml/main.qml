/*
 * Copyright (c), 2022-2024, Mist Studio.
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtCore

Item {
    id: root
    width: 200
    height: 600
    visible: true

    Settings {
        id: settings
        property bool firstRun: true
        property var wifi_card: root.wifi_card
        property int wifi_channel: root.wifi_channel
        property int wifi_bandwidth: root.wifi_bandwidth
        property var wifi_key: root.wifi_key
        property var player_codec: "rtph265depay ! h265parse ! avdec_h265"
    }

    property var wifi_card: "0bda:881a"
    property int wifi_channel: 100
    property int wifi_bandwidth: 20
    property var wifi_key: "etc/gs.key"
    property var player_codec: "rtph265depay ! h265parse ! avdec_h265"

    Component.onCompleted: {
        console.log(settings.firstRun)
        if (!settings.firstRun) {
            wifi_card = settings.wifi_card
            wifi_channel = settings.wifi_channel
            wifi_bandwidth = settings.wifi_bandwidth
            wifi_key = settings.wifi_key
            player_codec = settings.player_codec
        }
        settings.firstRun = false
    }

    function reset_settings() {
        settings.firstRun = true;
        settings.firstRun = false;
        settings.wifi_card = "0bda:881a"
        settings.wifi_channel = 1
        settings.wifi_bandwidth = 20
        settings.wifi_key = "/etc/gs.key"
        settings.player_codec = "rtph264depay ! h264parse ! avdec_h264"
    }

    FileDialog {
        id: fileDialog
        currentFolder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        onAccepted: {
            var o_gs_path = selectedFile.toString()
            var gs_path = o_gs_path.replace(/^file:\/\//, '')
            console.log(gs_path)
            wifi_key = gs_path
            settings.wifi_key = gs_path
        }
    }


    Item {
        width: parent.width
        height: 200
        anchors.top: parent.top

        ColumnLayout {
            anchors.fill: parent
            Item {
                id: item_choose
                width: parent.width
                height: 50
                RowLayout {
                    anchors.fill: parent
                    Label {
                        width: 80
                        height: 50
                        text: qsTr("Device")
                        font.pixelSize: 14
                        Layout.leftMargin: 10
                        Layout.alignment: Qt.AlignLeft
                    }
                    ComboBox {
                        id: comboBox
                        width: 100
                        height: 50
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                        Layout.rightMargin: 10
                        model: ListModel {
                            ListElement {
                                text: "0bda:881a"
                            }
                            ListElement {
                                text: "0bda:881b"
                            }
                            ListElement {
                                text: "0bda:881c"
                            }
                            ListElement {
                                text: "0bda:881d"
                            }
                        }
                        Component.onCompleted: {
                            for (var i = 0; i < model.count; i++) {
                                if (model.get(i).text === settings.wifi_card) {
                                    currentIndex = i
                                    break
                                }
                            }
                        }
                        onActivated: {
                            root.wifi_card = currentText
                            settings.wifi_card = currentText
                            console.log(root.wifi_card)
                        }
                    }
                }
            }
            Item {
                id: item_channel
                width: parent.width
                height: 50
                RowLayout {
                    anchors.fill: parent
                    Label {
                        width: 80
                        height: 50
                        text: qsTr("Channel")
                        font.pixelSize: 14
                        Layout.leftMargin: 10
                        Layout.alignment: Qt.AlignLeft
                    }
                    ComboBox {
                        id: comboBox_channel
                        width: 100
                        height: 50
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                        Layout.rightMargin: 10
                        model: ['1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12', '13', '32', '36', '40', '44', '48', '52', '56', '60', '64', '68', '96', '100', '104', '108', '112', '116', '120', '124', '128', '132', '136', '140', '144', '149', '153', '157', '161', '169', '173', '177']
                        Component.onCompleted: {
                            for (var i = 0; i < model.length; i++) {
                                if (model[i] === settings.wifi_channel.toString()) {
                                    currentIndex = i;
                                    break;
                                }
                            }
                        }
                        onActivated: {
                            root.wifi_channel = currentText
                            settings.wifi_channel = currentText
                            console.log(root.wifi_channel)
                        }
                    }
                }
            }
            Item {
                id: item_wifi_bandwidth
                width: parent.width
                height: 50
                RowLayout {
                    anchors.fill: parent
                    Label {
                        width: 80
                        height: 50
                        text: qsTr("Bandwidth")
                        font.pixelSize: 14
                        Layout.leftMargin: 10
                        Layout.alignment: Qt.AlignLeft
                    }
                    ComboBox {
                        id: comboBox_bandwidth
                        width: 100
                        height: 50
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                        Layout.rightMargin: 10
                        model: ['20MHz', '40MHz']
                        Component.onCompleted: {
                            for (var i = 0; i < model.length; i++) {
                                var bd = settings.wifi_bandwidth + "MHz"
                                if (model[i] === bd) {
                                    currentIndex = i;
                                    break;
                                }
                            }
                        }
                        onActivated: {
                            root.wifi_bandwidth = currentText === '20MHz' ? 20 : 40
                            settings.wifi_bandwidth = currentText === '20MHz' ? 20 : 40
                            console.log(root.wifi_bandwidth)
                        }
                    }
                }
            }

            Item {
                id: item_wifi_key
                width: parent.width
                height: 50
                RowLayout {
                    anchors.fill: parent
                    Label {
                        width: 80
                        height: 50
                        text: qsTr("key")
                        font.pixelSize: 14
                        Layout.leftMargin: 10
                        Layout.alignment: Qt.AlignLeft
                    }
                    Item {
                        width: root.width - 40 - 10
                        height: 30
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                        Layout.rightMargin: 10
                        Button {
                            anchors.fill: parent
                            id: btn_choose_gs_key
                            text: root.wifi_key
                            property var key_path: root.wifi_key
                            onClicked: {
                                fileDialog.open()
                            }
                            Component.onCompleted: {

                            }
                        }
                    }
                }

            }
            Item {
                width: 200
                height: 80
                Button {
                    id: wfb_btn1
                    text: isRunning ? qsTr("stop wfb") : qsTr("start wfb")
                    anchors.fill: parent
                    property var isRunning: false
                    onClicked: {
                        if (isRunning) {
                            wfbNG.stop()
                            console.log("stop wfb")
                            isRunning = !isRunning
                        } else {
                            wfbNG.start(root.wifi_card, root.wifi_channel, root.wifi_bandwidth, root.wifi_key)
                            console.log("start wfb")
                            isRunning = !isRunning
                        }
                    }
                }
            }


            Item {
                id: item_codec
                width: parent.width
                height: 50
                RowLayout {
                    anchors.fill: parent
                    Label {
                        width: 80
                        height: 50
                        text: qsTr("Codec")
                        font.pixelSize: 14
                        Layout.leftMargin: 10
                        Layout.alignment: Qt.AlignLeft
                    }
                    ComboBox {
                        id: comboBox_codec
                        width: 100
                        height: 50
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                        Layout.rightMargin: 10
                        model: ["h265", "h264"]
                        onActivated: {
                            root.player_codec = (currentText === "h265") ? "rtph265depay ! h265parse ! avdec_h265" : "rtph264depay ! h264parse ! avdec_h264"
                            settings.player_codec = (currentText === "h265") ? "rtph265depay ! h265parse ! avdec_h265" : "rtph264depay ! h264parse ! avdec_h264"
                            console.log(root.player_codec)
                        }
                        Component.onCompleted: {
                            var codec = (settings.player_codec === "rtph265depay ! h265parse ! avdec_h265") ? "h265" : "h264";
                            for (var i = 0; i < model.length; i++) {
                                if (model[i] === codec) {
                                    currentIndex = i;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            Item {
                width: 200
                height: 80
                Button {
                    id: wfb_btn3
                    text: isRunning ? "stop video" : "show video"
                    anchors.fill: parent
                    property var isRunning: false
                    onClicked: {
                        if (isRunning) {
                            player.stop();
                        } else {
                            player.pause();
                            player.setPlayUri("udpsrc name=ms-udp port=5600 ! " + player_codec);
                            player.play();
                        }
                        isRunning = !isRunning
                    }
                }
            }
        }
    }

    Image {
        id: rest_image
        width: 20
        height: 20
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.rightMargin: 10
        anchors.bottomMargin: 10
        source: "qrc:/assets/refresh.svg"

        MouseArea {
            id: rest_image_mouse
            anchors.fill: parent
            hoverEnabled: true
            onClicked: {
                reset_settings()
            }
            onEntered: {
                rest_image_tooltip.visible = true
            }
            onExited: {
                rest_image_tooltip.visible = false
            }
        }
        ToolTip {
            id: rest_image_tooltip
            parent: rest_image
            visible: false
            text: qsTr("Reset settings")
            timeout: 1000
        }
    }
}
