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

AppletItem {
    id: root
    implicitWidth: {
        return quickpanel.width
    }
    implicitHeight: {
        return quickpanel.childrenRect.height
    }
    // implicitWidth: {
    //     return panelView.implicitWidth
    // }
    // implicitHeight: {
    //     return panelView.implicitHeight
    // }

    QuickPanelModel {
        id: quickpanelModel
    }

    StackView {
        id: panelView
        anchors.fill: parent
    }

    QuickPanel {
        id: quickpanel
        model: quickpanelModel
        onRequestShowSubPlugin: function (plugin) {
            // StackView to manage QuickPanel
            if (panelView.empty)
                panelView.push(quickpanel)

            var item = panelView.push(subPluginPageLoader,
                                      {
                                          pluginKey: plugin,
                                          model: quickpanelModel
                                      },
                                      StackView.PushTransition)
            item.requestBack.connect(function () {
                panelView.pop()
            })
        }
    }

    Component {
        id: subPluginPageLoader
        SubPluginPage { }
    }
}
