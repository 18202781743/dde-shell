// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "bubbleapplet.h"

#include "pluginfactory.h"

#include <QDBusConnection>
#include <QDBusPendingCall>
#include <QDBusReply>

#include <DDBusSender>

DS_BEGIN_NAMESPACE
namespace notification {

static DDBusSender bubbleInter()
{
    return DDBusSender().service("org.deepin.dde.Bubble1")
        .path("/org/deepin/dde/Bubble1")
        .interface("org.deepin.dde.Bubble1");
}

BubbleApplet::BubbleApplet(QObject *parent)
    : DApplet(parent)
{

}

void BubbleApplet::init()
{
}

D_APPLET_CLASS(BubbleApplet)

}
DS_END_NAMESPACE

#include "bubbleapplet.moc"
