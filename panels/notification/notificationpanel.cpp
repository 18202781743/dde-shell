// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "notificationpanel.h"

#include "bubblemodel.h"
#include "pluginfactory.h"

#include <QDBusConnection>
#include <QTimer>
#include <QLoggingCategory>
#include <QDBusConnection>
#include <QDBusError>
#include <QTimer>

DS_BEGIN_NAMESPACE
namespace notification {

Q_LOGGING_CATEGORY(notificationLog, "dde.shell.notification")

NotificationPanel::NotificationPanel(QObject *parent)
    : DPanel(parent)
    , m_bubbles(new BubbleModel(this))
{
}

NotificationPanel::~NotificationPanel()
{
    if (m_interproxy) {
        m_interproxy->replaceNotificationBubble(false);
    }
}

bool NotificationPanel::load()
{
    return DPanel::load();
}

bool NotificationPanel::init()
{
    DPanel::init();

    m_interproxy = new NotificationInterProxy(this);
    if (!m_interproxy->replaceNotificationBubble(true)) {
        return false;
    }
    QObject::connect(m_interproxy, &NotificationInterProxy::ShowBubble, this, &NotificationPanel::onShowBubble);

    QTimer *timer = new QTimer(this);
    timer->setInterval(1000);
//    timer->setSingleShot(true);
    timer->start(1000);
    connect(timer, &QTimer::timeout, this, [this, timer]() {
        if (m_bubbles->rowCount(QModelIndex()) >= 5) {
            timer->stop();
            return;
            m_bubbles->clear();
        }
        auto i = m_bubbles->rowCount(QModelIndex());
        auto item = new BubbleItem(QString("title") + QString::number(i),
                                   QString("text") + QString::number(i),
                                   "deepin-manual");
        if (i == 0) {
            item->setParams("appName", i, {"op1", "mu1"}, {}, 0, 3, {});
        } else if (i == 1) {
            item->setParams("appName", i, {"op1", "mu1", "op2", "mu2"}, {}, 0, 3, {});
        } else if (i == 2) {
            item->setParams("appName", i, {"op1", "mu1", "op2", "mu2", "op3", "mu3"}, {}, 0, 3, {});
        } else if (i == 3) {
            item->setParams("appName", i, {"op1", "mu1", "op2", "mu2", "op3", "mu3"}, {}, 0, 3, {});
        } else if (i == 4) {
            item->setParams("appName", i, {"op1", "mu1", "default", "mu2", "op3", "mu3"}, {}, 0, 3, {});
        }

        m_bubbles->push(item);
    });
    return true;
}

bool NotificationPanel::visible() const
{
    return m_visible;
}


void NotificationPanel::hideNotification()
{
    setVisible(false);
}

void NotificationPanel::onShowBubble(const QString &appName, uint replaceId,
                                     const QString &appIcon, const QString &summary,
                                     const QString &body, const QStringList &actions,
                                     const QVariantMap hints, int expireTimeout,
                                     const QVariantMap bubbleParams)
{
    qDebug() << "Received bubble" << appName << replaceId << appIcon << summary << body
             << actions << hints << expireTimeout << bubbleParams;
    const auto id = bubbleParams["id"].toUInt();
    auto bubble = new BubbleItem(summary,
                               body,
                               appIcon);
    QObject::connect(bubble, &BubbleItem::timeout, this, &NotificationPanel::onBubbleTimeout);
    bubble->setParams(appName, id, actions, hints, replaceId, expireTimeout, bubbleParams);
    for (auto item : m_bubbles->items()) {
        if (item->m_replaceId == bubble->m_replaceId &&
            item->m_appName == bubble->m_appName) {
        }
    }
    m_bubbles->push(bubble);
}

void NotificationPanel::onBubbleTimeout()
{
    auto bubble = qobject_cast<BubbleItem *>(sender());
    if (!bubble)
        return;

    m_bubbles->remove(bubble);
    m_interproxy->handleBubbleEnd(1, bubble->id());
}

void NotificationPanel::onActionInvoked()
{

}

void NotificationPanel::showNotification()
{
    setVisible(true);
}

void NotificationPanel::setVisible(const bool visible)
{
    if (visible == m_visible)
        return;
    m_visible = visible;
    Q_EMIT visibleChanged();
}

D_APPLET_CLASS(NotificationPanel)

BubbleModel *NotificationPanel::bubbles() const
{
    return m_bubbles;
}

void NotificationPanel::actionInvoke(int bubbleIndex, const QString &actionId)
{
    auto bubble = m_bubbles->items().at(bubbleIndex);
    if (!bubble)
        return;

    m_bubbles->remove(bubbleIndex);
    QVariantMap selectedHints;
    selectedHints["actionId"] = actionId;
    QVariantMap bubbleParams;
    selectedHints["replaceId"] = bubble->m_replaceId;
    m_interproxy->handleBubbleEnd(5, bubble->id(), bubbleParams, selectedHints);
}

}
DS_END_NAMESPACE

#include "notificationpanel.moc"
