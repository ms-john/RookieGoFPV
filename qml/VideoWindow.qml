/*
* Copyright (c), 2022-2024, Mist Studio.
*/

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtCore
import com.pocketfpv.windowHelper

Item {
    WindowContainer {
        WindowHelper {
            id: windowHelper
        }
        window: Window {
            id: video_window
            width: mainWindow.width
            height: mainWindow.height
            color: "transparent"
            visible: true
            Component.onCompleted: {
                videoStream.playerId = windowHelper.getWinId(video_window);
            }
        }
    }
}
