// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtWayland.Compositor

import org.deepin.ds 1.0
import org.deepin.ds.dock 1.0
import org.deepin.dtk 1.0

Control {
    id: root
    property alias color: view.color
    property alias shellSurface: surface.shellSurface

    Rectangle {
        id: view
        anchors.fill: parent

        color: "red"
        radius: 18
        opacity: 0.3
    }
    Item {
        anchors.fill: parent
        ShellSurfaceItem {
            id: surface
            anchors.centerIn: parent
        }
    }

    background: BoxPanel {
        color2: color1
        radius: 18
    }
}
