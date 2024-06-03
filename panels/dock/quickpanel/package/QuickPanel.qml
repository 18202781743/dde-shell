// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQml.Models
import QtQml

import org.deepin.ds 1.0
import org.deepin.dtk 1.0
import org.deepin.ds.dock 1.0

Item {
    id: root
    implicitWidth: {
        return view.width
    }
    implicitHeight: {
        return view.childrenRect.height
    }
    readonly property int cellSize: 70
    readonly property int columnCellCounts: 4
    required property var model

    signal requestShowSubPlugin(var plugin)

    Flow {
        id: view
        width: 330
        anchors.fill: parent
        spacing: 10
        Repeater {
            anchors.fill: parent
            model: viewModel
        }
    }

    DelegateModel {
        id: viewModel
        model: root.model

        delegate: QuickPanelItem {
            id: quickpanelItem
            width: {
                if (model.type === 4)
                    return 310
                // if (model.type === 2)
                    return 150
                return 70
            }
            height: 60
            shellSurface: model.surface

            // clickedEnabled: model.type === 2
            onClicked: {
                console.log("request show subplugin:", model.pluginName)
                requestShowSubPlugin(model.pluginName)
            }
        }
    }
}
