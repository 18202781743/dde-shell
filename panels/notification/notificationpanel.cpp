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
    QDBusConnection bus = QDBusConnection::sessionBus();
    if (!bus.registerService("org.deepin.dde.Shell")) {
        qWarning() << "register failed" << bus.lastError().message();
    }

    DPanel::load();
}

void NotificationPanel::init()
{
    auto bus = QDBusConnection::sessionBus();
    bus.registerObject(QStringLiteral("/org/deepin/notificationService"),
                       this,
                       QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllSignals);

    DPanel::init();

    QTimer *timer = new QTimer(this);
    timer->setInterval(1000);
    timer->setSingleShot(false);
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
