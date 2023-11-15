// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15

import org.deepin.ds 1.0
import org.deepin.dtk 1.0 as D

D.Control {
    property var bubble

    MouseArea {
        anchors.fill: parent

        onClicked: {
            if (!bubble.hasDefaultAction)
                return

            console.log("default action", bubble.index)
            Applet.defaultActionInvoke(bubble.index)
        }
        property bool longPressed
        onPressAndHold: {
            longPressed = true
        }
        onPositionChanged: {
            if (longPressed) {
                longPressed = false
                console.log("delay process", bubble.index)
                Applet.delayProcess(bubble.index)
            }
        }
    }
    contentItem: RowLayout {

        D.QtIcon {
            Layout.leftMargin: 10
            sourceSize: Qt.size(32, 32)
            name: bubble.iconName
        }

        ColumnLayout {
            Layout.leftMargin: 10
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumWidth: 400
            Text {
                Layout.alignment: Qt.AlignLeft
                elide: Text.ElideRight
                text: bubble.title
                Layout.fillWidth: true
                maximumLineCount: 1
                font: D.DTK.fontManager.t4
            }

            Text {
                Layout.alignment: Qt.AlignLeft
                text: bubble.text
                Layout.fillWidth: true
                elide: Text.ElideRight
                maximumLineCount: 1
            }
        }

        Loader {
            Layout.alignment: Qt.AlignRight
            active: bubble.hasDisplayAction
            sourceComponent: BubbleAction {
                bubble: control.bubble
                onActionInvoked: function(actionId) {
                    console.log("action", actionId, bubble.index)
                    Applet.actionInvoke(bubble.index, actionId)
                }
            }
        }

        D.ActionButton {
            icon.name: "window_close"
            Layout.alignment: Qt.AlignRight
            onClicked: {
                console.log("close", bubble.index)
                Applet.close(bubble.index)
            }
        }
    }
}
