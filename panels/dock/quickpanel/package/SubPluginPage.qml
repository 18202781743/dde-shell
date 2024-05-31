// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import org.deepin.ds 1.0
import org.deepin.dtk 1.0
import org.deepin.ds.dock 1.0

Item {
    id: root
    implicitWidth: childrenRect.width
    implicitHeight: childrenRect.height

    required property var pluginKey
    required property var model
    signal requestBack()

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // header
        RowLayout {
            Layout.fillWidth: true
            Layout.leftMargin: 8
            Layout.topMargin: 10
            Layout.rightMargin: 12
            ActionButton {
                icon.name: "arrow_ordinary_left"
                icon.width: 16
                icon.height: 16
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                onClicked: {
                    requestBack()
                }
            }

            Item { Layout.fillWidth: true; Layout.preferredHeight: 1 }

            Label {
                Layout.alignment: Qt.AlignHCenter
                text: model.getTitle(pluginKey)
            }

            Item { Layout.fillWidth: true; Layout.preferredHeight: 1 }
        }

        // content
        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true

            Rectangle {
                Layout.fillHeight: true
                Layout.fillWidth: true
                color: model.getSurfaceItem(pluginKey)
            }
        }
    }
}
