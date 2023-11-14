// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15

import org.deepin.ds 1.0
import org.deepin.dtk 1.0 as D

Control {
    id: control
    property var bubble

    width: {
        if (bubble.level <= 2)
            return 400
        if (bubble.level === 3)
            return 350
        if (bubble.level === 4)
            return 300
        return 100
    }
    height: 50
    contentItem: RowLayout {
        D.QtIcon {
            Layout.leftMargin: 20
            sourceSize: Qt.size(32, 32)
            name: bubble.iconName
        }

        ColumnLayout {
            Layout.leftMargin: 10
            Layout.fillWidth: true
            Text {
                Layout.alignment: Qt.AlignLeft
                text: bubble.title
                Layout.fillWidth: true
            }

            Text {
                Layout.alignment: Qt.AlignLeft
                text: bubble.text
                Layout.fillWidth: true
            }
        }

        Loader {
            Layout.alignment: Qt.AlignRight
            active: bubble.hasAction
            sourceComponent: BubbleAction {
                bubble: control.bubble
                onActionInvoked: function(actionId) {
                    console.log(actionId, index)
                    Applet.actionInvoke(bubble.index, actionId)
                }
            }
        }

        D.ActionButton {
            icon.name: "window_close"
            Layout.alignment: Qt.AlignRight
            onClicked: {
                Applet.bubbles.remove(bubble.index)
            }
        }
    }

    SequentialAnimation on y {
        running: bubble.index > -1
        PropertyAnimation {
            to: bubble.index <= 2 ? bubble.index * (50 + 10) : (50 + 10) * 2 + (bubble.index - 2 ) * (10 + 10);
            easing.type: Easing.InQuad
        }
    }
    z: bubble.level <= 1 ? 0 : 1 - bubble.level

    background: Rectangle {
        color: {
            if (bubble.level === 1)
                return "#3265d3"
            if (bubble.level === 2)
                return "#326500"
            if (bubble.level === 3)
                return "#320000"
            if (bubble.level === 4)
                return "#300000"
            return "gray"
        }
        radius: 18
        opacity: {
            if (bubble.level === 1)
                return 0.8
            return 1
        }
    }
}
