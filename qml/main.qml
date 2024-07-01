import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    width: 200
    height: 600
    visible: true
    Rectangle {
        width:100
        height:parent.height
        anchors.left: parent.left
        color: "transparent"
        border.width: 2
        border.color: "black"

        Button {
            id: test_btn1
            text: "this is a play button"
            width: 100
            height: 100
            anchors.left: parent.left
            onClicked: {
                player.play();
            }
        }
        Button {
            id: test_btn2
            text: "this is a stop button"
            width: 100
            height: 100
            anchors.top: test_btn1.bottom
            onClicked: {
                player.stop();
            }
        }
        Button {
            id: test_btn3
            text: "change"
            width: 100
            height: 100
            anchors.top: test_btn2.bottom
            onClicked: {
                player.pause();
                player.setPlayUri("videotestsrc pattern=1 ! video/x-raw,format=NV12,width=1920,height=1080,framerate=30/1");
                player.play();
            }
        }
    }

    Rectangle {
        width:100
        height:parent.height
        anchors.right: parent.right
        color: "transparent"
        border.width: 2
        border.color: "black"

        Button {
            id: wfb_btn1
            text: "start wfb"
            width: 100
            height: 100
            anchors.left: parent.left
            onClicked: {
                ////0bda:881a
                wfbNG.Start("0bda:881a", 100, 0, "/Users/yinyong/WorkSpace/QtProjects/RookieGoFPV/gs.key")
            console.log("start wfb")
            }
        }
        Button {
            id: wfb_btn2
            text: "this is a stop button"
            width: 100
            height: 100
            anchors.top: wfb_btn1.bottom
            onClicked: {
                player.stop();
            }
        }
        Button {
            id: wfb_btn3
            text: "show video"
            width: 100
            height: 100
            anchors.top: wfb_btn2.bottom
            onClicked: {
                player.pause();
                player.setPlayUri("udpsrc name=ms-udp port=5600 ! rtph265depay ! h265parse ! avdec_h265");
                player.play();
            }
        }
    }
}
