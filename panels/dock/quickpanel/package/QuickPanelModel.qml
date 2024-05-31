// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls

import org.deepin.ds.dock 1.0
import org.deepin.ds.dock.quickpanel 1.0

PluginOrderProxyModel {
    id: root

    sourceModel: DockCompositor.trayPluginSurfaces
    // sourceModel: ListModel {
    //     ListElement {
    //         order: 2
    //         color: "gray"
    //         type: 1
    //         pluginName: "network"
    //         title: "network"
    //     }
    //     ListElement {
    //         order: 2
    //         color: "blue"
    //         type: 2
    //         pluginName: "bluetooth"
    //         title: "plugin-1"
    //     }
    //     ListElement {
    //         order: 1
    //         color: "gray"
    //         type: 1
    //         pluginName: "wireless-casting"
    //         title: "plugin-1"
    //     }
    //     ListElement {
    //         order: 2
    //         color: "gray"
    //         type: 2
    //         pluginName: "grand-search"
    //         title: "plugin-1"
    //     }
    //     ListElement {
    //         order: 2
    //         color: "gray"
    //         type: 1
    //         pluginName: "eye-comfort-mode"
    //         title: "eye-comfort-mode"
    //     }
    //     ListElement {
    //         order: -1
    //         color: "gray"
    //         type: 1
    //         pluginName: "airplane-mode"
    //         title: "airplane-mode"
    //     }
    //     ListElement {
    //         order: 2
    //         color: "gray"
    //         type: 1
    //         pluginName: "dnd-mode"
    //         title: "dnd-mode"
    //     }
    //     ListElement {
    //         order: 2
    //         color: "gray"
    //         type: 1
    //         pluginName: "plugin-7"
    //         title: "plugin-1"
    //     }
    //     ListElement {
    //         order: 2
    //         color: "red"
    //         type: 4
    //         pluginName: "plugin-8"
    //         title: "plugin-1"
    //     }
    //     ListElement {
    //         order: 2
    //         color: "red"
    //         type: 4
    //         pluginName: "plugin-9"
    //         title: "plugin-1"
    //     }
    // }
}
