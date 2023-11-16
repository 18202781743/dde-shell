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
    if (m_interproxy && m_interproxy->isValid()) {
        qCInfo(notificationLog) << "Cancle ReplaceBubble of osd's service.";
        if (!m_interproxy->replaceNotificationBubble(false)) {
            qCWarning(notificationLog) << "Failed to cancle ReplaceBubble of osd's service.";
        }
    }
}

bool NotificationPanel::load()
{
    return DPanel::load();
}

bool NotificationPanel::init()
{
    DPanel::init();

    m_interproxy = new NotificationProxy(this);
    if (!m_interproxy->isValid()) {
        qCWarning(notificationLog) << "Failed to fetch notification service's handler.";
        return false;
    }

    qCInfo(notificationLog) << "Intercept ReplaceBubble of osd's service.";
    if (!m_interproxy->replaceNotificationBubble(true)) {
        return false;
    }

    QObject::connect(m_interproxy, &NotificationProxy::ShowBubble, this, &NotificationPanel::onShowBubble);
    QObject::connect(m_bubbles, &BubbleModel::rowsInserted, this, &NotificationPanel::onBubbleCountChanged);
    QObject::connect(m_bubbles, &BubbleModel::rowsRemoved, this, &NotificationPanel::onBubbleCountChanged);

    QTimer *timer = new QTimer(this);
    timer->setInterval(1000);
//    timer->setSingleShot(true);
    timer->start(1000);
    connect(timer, &QTimer::timeout, this, [this, timer]() {
        if (m_bubbles->items().count() >= 5) {
            timer->stop();
            return;
            m_bubbles->clear();
        }
        auto i = m_bubbles->items().count();
        auto item = new BubbleItem(QString("title") + QString::number(i),
                                   QString("text") + QString::number(i),
                                   "deepin-manual");
//        QObject::connect(item, &BubbleItem::timeout, this, &NotificationPanel::onBubbleTimeout);

        if (i == 0) {
            item->setParams("dde-control-center", i, {"op1", "mu1"}, {}, 0, -1, {});
        } else if (i == 1) {
            item->setParams("dde-control-center", i, {"op1", "mu1", "op2", "mu2"}, {}, 0, -1, {});
        } else if (i == 2) {
            item->setParams("appName", i, {"op1", "mu1", "op2", "mu2", "op3", "mu3"}, {}, 0, 0, {});
        } else if (i == 3) {
            item->setParams("appName", i, {"op1", "mu1", "op2", "mu2", "op3", "mu3"}, {}, 0, 0, {});
        } else if (i == 4) {
            item->setParams("appName", i, {"op1", "mu1", "default", "mu2", "op3", "mu3"}, {}, 0, 0, {});
        } else {
            item->setParams("appName", i, {"op1", "mu1"}, {}, 0, -1, {});
        }

        m_bubbles->push(item);
    });
    return true;
}

bool NotificationPanel::visible() const
{
    return m_visible;
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
    m_interproxy->handleBubbleEnd(NotificationProxy::Expired, bubble->id());
}

void NotificationPanel::onActionInvoked()
{

}

void NotificationPanel::showNotification()
{
    setVisible(true);
}

BubbleItem *NotificationPanel::bubbleItem(int index)
{
    if (index < 0 || index >= m_bubbles->items().count())
        return nullptr;
    return m_bubbles->items().at(index);
}

void NotificationPanel::setVisible(const bool visible)
{
    if (visible == m_visible)
        return;
    m_visible = visible;
    Q_EMIT visibleChanged();
}

BubbleModel *NotificationPanel::bubbles() const
{
    return m_bubbles;
}

void NotificationPanel::defaultActionInvoke(int bubbleIndex)
{
    auto bubble = bubbleItem(bubbleIndex);
    if (!bubble)
        return;

    QVariantMap selectedHints;
    selectedHints["actionId"] = bubble->defaultActionId();
    m_interproxy->handleBubbleEnd(NotificationProxy::Action, bubble->id(), bubble->toMap(), selectedHints);
    m_bubbles->remove(bubbleIndex);
}

void NotificationPanel::actionInvoke(int bubbleIndex, const QString &actionId)
{
    auto bubble = bubbleItem(bubbleIndex);
    if (!bubble)
        return;

    QVariantMap selectedHints;
    selectedHints["actionId"] = actionId;
    QVariantMap bubbleParams;
    selectedHints["replaceId"] = bubble->m_replaceId;
    m_interproxy->handleBubbleEnd(NotificationProxy::Action, bubble->id(), bubbleParams, selectedHints);
    m_bubbles->remove(bubbleIndex);
}

void NotificationPanel::close(int bubbleIndex)
{
    auto bubble = bubbleItem(bubbleIndex);
    if (!bubble)
        return;

    m_interproxy->handleBubbleEnd(NotificationProxy::Dismissed, bubble->id(), {}, {});
    m_bubbles->remove(bubbleIndex);
}

void NotificationPanel::delayProcess(int bubbleIndex)
{
    auto bubble = bubbleItem(bubbleIndex);
    if (!bubble)
        return;

    m_interproxy->handleBubbleEnd(NotificationProxy::NotProcessedYet, bubble->id(), bubble->toMap(), {});
    m_bubbles->remove(bubbleIndex);
}

void NotificationPanel::onBubbleCountChanged()
{
    bool isEmpty = m_bubbles->items().isEmpty();
    setVisible(!isEmpty);
}

D_APPLET_CLASS(NotificationPanel)

}
DS_END_NAMESPACE

#include "notificationpanel.moc"
