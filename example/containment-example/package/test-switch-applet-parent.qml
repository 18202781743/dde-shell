// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.15
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11

import org.deepin.ds 1.0

ContainmentItem {
    id: root
    objectName: "containment item"
    property bool selectedTest1: false
    Loader {
        sourceComponent: selectedTest1 ? test1 :test2
    }
    property var itemModel: []
    Component.onCompleted: {
        var tmp = []
        tmp[0] = rect1
        tmp[1] = rect2
        tmp[2] = rect3
        itemModel = tmp
    }
    Rectangle {id: rect1; implicitHeight: 30; implicitWidth: 30; color: "red" }
    Rectangle {id: rect2; implicitHeight: 30; implicitWidth: 30; color: "green" }
    Rectangle {id: rect3; implicitHeight: 30; implicitWidth: 30; color: "blue" }

    Component {
        id: test1

        Row {
            height: 200
            Repeater {
                model: itemModel
                delegate: Control {
                    contentItem: modelData
                    Component.onCompleted: {
                        modelData.parent = this
                    }

                    background: Rectangle {
                        color: "gray"
                        width: 40
                        height: 40
                    }
                }
            }
        }
    }

    Component {
        id: test2

        Column {
            width: 200
            Repeater {
                model: itemModel
                delegate: Control {
                    contentItem: modelData
                    Component.onCompleted: {
                        modelData.parent = this
                    }
                    background: Rectangle {
                        color: "gray"
                        width: 40
                        height: 40
                    }
                }
            }
        }
    }
    Timer {
        running: true
        interval: 1000
        repeat: true
        onTriggered: {
            selectedTest1 = !selectedTest1
        }
    }
}
