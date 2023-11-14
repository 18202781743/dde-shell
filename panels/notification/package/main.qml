// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15

import org.deepin.ds 1.0
import org.deepin.dtk 1.0 as D

Window {
    id: root
    visible: true
    width: 500
    height: 600
//    DLayerShellWindow.topMargin: 10
//    DLayerShellWindow.leftMargin: 800
//    DLayerShellWindow.layer: DLayerShellWindow.LayerOverlay
//    DLayerShellWindow.anchors: DLayerShellWindow.AnchorTop

    Repeater  {
        width: root.width
        model: Applet.bubbles
        delegate: BubbleItem {
            anchors.horizontalCenter: parent.horizontalCenter
            bubble: model
        }
    }
//    color: "transparent"
}
