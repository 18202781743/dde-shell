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

void NotificationPanel::load()
{
    DPanel::load();
}

void NotificationPanel::init()
{
    DPanel::init();

    m_interproxy = new NotificationInterProxy(this);
    QObject::connect(m_interproxy, &NotificationInterProxy::ShowBubble, this, &NotificationPanel::onShowBubble);

    QTimer *timer = new QTimer(this);
    timer->setInterval(1000);
    timer->setSingleShot(true);
//    timer->start(1000);
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
        m_bubbles->push(item);
    });
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

}
DS_END_NAMESPACE

#include "notificationpanel.moc"
