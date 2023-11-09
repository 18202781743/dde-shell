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
    property string text
    property string title
    property string iconName
    property int level
    property int index

    width: {
        if (control.level <= 2)
            return 400
        if (control.level === 3)
            return 350
        if (control.level === 4)
            return 300
        return 100
    }
    height: 50
    contentItem: RowLayout {
        D.QtIcon {
            Layout.leftMargin: 20
            sourceSize: Qt.size(32, 32)
            name: control.iconName
        }
        Text {
            text: control.text
        }
        D.ActionButton {
            icon.name: "window_close"
            Layout.alignment: Qt.AlignRight
            onClicked: {
                Applet.bubbles.remove(control.index)
            }
        }
        D.ActionButton {
            icon.name: "window_close"
            Layout.alignment: Qt.AlignRight
            onClicked: {
                Applet.bubbles.remove(control.index)
            }
        }
    }

    SequentialAnimation on y {
        running: control.index > -1
        PropertyAnimation {
            to: control.index <= 2 ? control.index * (50 + 10) : (50 + 10) * 2 + (control.index - 2 ) * (10 + 10);
            easing.type: Easing.InQuad
        }
    }
    z: control.level <= 1 ? 0 : 1 - control.level

    background: Rectangle {
        color: {
            if (control.level === 1)
                return "#3265d3"
            if (control.level === 2)
                return "#326500"
            if (control.level === 3)
                return "#320000"
            if (control.level === 4)
                return "#300000"
            return "gray"
        }
        radius: 18
        opacity: {
            if (control.level === 1)
                return 0.8
            return 1
        }
    }
}
