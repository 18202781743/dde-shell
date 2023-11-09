// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "panel.h"
#include "bubblemodel.h"
#include "notificationinterproxy.h"
#include <QQuickItem>

DS_BEGIN_NAMESPACE
namespace notification {

class NotificationPanel : public DPanel
{
    Q_OBJECT
    Q_PROPERTY(bool visible READ visible NOTIFY visibleChanged FINAL)
    Q_PROPERTY(BubbleModel *bubbles READ bubbles CONSTANT FINAL)
    Q_CLASSINFO("D-Bus Interface", "org.deepin.notificationService")
public:
    explicit NotificationPanel(QObject *parent = nullptr);

    virtual void load() override;
    virtual void init() override;

    bool visible() const;
    BubbleModel *bubbles() const;

public Q_SLOTS:

Q_SIGNALS:
    void visibleChanged();

private Q_SLOTS:
    void hideNotification();
    void onShowBubble(const QString &appName, uint replaceId,
                      const QString &appIcon, const QString &summary,
                      const QString &body, const QStringList &actions,
                      const QVariantMap hints, int expireTimeout,
                      const QVariantMap bubbleParams);

    void onBubbleTimeout();
private:
    void setVisible(const bool visible);
    void showNotification();

private:
    bool m_visible = false;
    BubbleModel *m_bubbles = nullptr;
    NotificationInterProxy *m_interproxy = nullptr;
};

}
DS_END_NAMESPACE
