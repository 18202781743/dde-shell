// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "notificationinterproxy.h"

#include <QDBusConnection>

#include <DDBusSender>

DS_BEGIN_NAMESPACE
namespace notification {

static DDBusSender notificationInter()
{
    return DDBusSender()
        .service("org.deepin.dde.Notification1")
        .path("/org/deepin/dde/Notification1")
        .interface("org.deepin.dde.Notification1");
}

NotificationInterProxy::NotificationInterProxy(QObject *parent)
    : QObject(parent)
{
    auto bus = QDBusConnection::sessionBus();
    bool res = bus.connect("org.deepin.dde.Notification1",
                "/org/deepin/dde/Notification1",
                "org.deepin.dde.Notification1",
                "ShowBubble",
                this, SLOT(ShowBubble(const QString &, uint, const QString &, const QString &, const QString &, const QStringList &, const QVariantMap &, int, const QVariantMap &)));

    if (!res) {
        qWarning() << "Failed to connect Notification's ShowBubble signal" << bus.lastError();
    }
}

void NotificationInterProxy::replaceNotificationBubble(const QString &service)
{
    notificationInter().method("ReplaceBubble").arg(service).call();
}

void NotificationInterProxy::handleBubbleEnd(int type, const QVariantMap &bubbleParams, const QVariantMap &selectedHints)
{
    notificationInter().method("HandleBubbleEnd").arg(static_cast<uint>(type)).arg(bubbleParams).arg(selectedHints).call();
}

}
DS_END_NAMESPACE
