import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    width: 200
    height: 600
    visible: true

    property var wifi_card: "0bda:881a"
    property int wifi_channel: 100
    property int wifi_bandwidth: 20
    property var wifi_key: "etc/gs.key"


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
                        text: "Device"
                        font.pixelSize: 14
                        Layout.leftMargin: 20
                        Layout.alignment: Qt.AlignLeft
                    }
                    ComboBox {
                        id: comboBox
                        width: 100
                        height: 50
                        Layout.alignment: Qt.AlignRight
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
                        onActivated: {
                            root.wifi_card = currentText
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
                        text: "Channel"
                        font.pixelSize: 14
                        Layout.leftMargin: 20
                        Layout.alignment: Qt.AlignLeft
                    }
                    ComboBox {
                        id: comboBox_channel
                        width: 100
                        height: 50
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                        flat: true
                        model: ['1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12', '13',
                            '32', '36', '40', '44', '48', '52', '56', '60', '64', '68', '96', '100', '104', '108', '112', '116', '120',
                            '124', '128', '132', '136', '140', '144', '149', '153', '157', '161', '169', '173', '177'
                        ]
                        onActivated: {
                            root.wifi_channel = currentText
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
                        text: "Bandwidth"
                        font.pixelSize: 14
                        Layout.leftMargin: 20
                        Layout.alignment: Qt.AlignLeft
                    }
                    ComboBox {
                        id: comboBox_bandwidth
                        width: 100
                        height: 50
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                        flat: true
                        model: ['20MHz', '40MHz']
                        onActivated: {
                            root.wifi_bandwidth = currentText === '20MHz' ? 20 : 40
                            console.log(root.wifi_bandwidth)
                        }
                        Component.onCompleted: {
                            currentIndex = 0
                        }
                    }
                }
            }
            Item {
                width: 200
                height: 80
                Button {
                    id: wfb_btn1
                    text: isRunning ? "stop wfb" : "start wfb"
                    anchors.fill: parent
                    property var isRunning: false
                    onClicked: {
                        if (isRunning) {
                            wfbNG.stop()
                            console.log("stop wfb")
                            isRunning = !isRunning
                        }else {
                            wfbNG.start(root.wifi_card, root.wifi_channel, root.wifi_bandwidth, root.wifi_key)
                            console.log("start wfb")
                            isRunning = !isRunning
                        }
                    }
                }
            }

            Item {
                width: 200
                height: 80
                Button {
                    id: wfb_btn2
                    text: "this is a stop button"
                    anchors.fill: parent
                    onClicked: {
                        player.stop();
                    }
                }
            }
            Item {
                width: 200
                height: 80
                Button {
                    id: wfb_btn3
                    text: "show video"
                    anchors.fill: parent
                    onClicked: {
                        player.pause();
                        player.setPlayUri("udpsrc name=ms-udp port=5600 ! rtph265depay ! h265parse ! avdec_h265");
                        player.play();
                    }
                }
            }
        }


    }
}
