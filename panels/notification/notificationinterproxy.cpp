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

NotificationProxy::NotificationProxy(QObject *parent)
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

bool NotificationProxy::replaceNotificationBubble(bool replace)
{
    auto reply = notificationInter().method("ReplaceBubble").arg(replace).call();
    reply.waitForFinished();
    return !reply.isError();
}

void NotificationProxy::handleBubbleEnd(int type, int id)
{
    return handleBubbleEnd(type, id, {}, {});
}

void NotificationProxy::handleBubbleEnd(int type, int id, const QVariantMap &bubbleParams, const QVariantMap &selectedHints)
{
    notificationInter().method("HandleBubbleEnd").arg(static_cast<uint>(type)).arg(static_cast<uint>(id)).arg(bubbleParams).arg(selectedHints).call();
}

}
DS_END_NAMESPACE
