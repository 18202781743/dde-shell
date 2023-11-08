// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "panel.h"
#include "bubblemodel.h"
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
private:
    void setVisible(const bool visible);
    void showNotification();

private:
    bool m_visible = false;
    BubbleModel *m_bubbles = nullptr;
};

}
DS_END_NAMESPACE
