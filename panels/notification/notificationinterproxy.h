// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "dsglobal.h"

#include <QObject>

DS_BEGIN_NAMESPACE
namespace notification {

class NotificationInterProxy : public QObject
{
    Q_OBJECT
public:
    explicit NotificationInterProxy(QObject *parent = nullptr);

    void replaceNotificationBubble(const QString &service);

    void handleBubbleEnd(int type, const QVariantMap &bubbleParams, const QVariantMap &selectedHints);

Q_SIGNALS:
    void ShowBubble(const QString &appName, uint replacesId,
                    const QString &appIcon, const QString &summary,
                    const QString &body, const QStringList &actions,
                    const QVariantMap hints, int expireTimeout,
                    const QVariantMap bubbleParams);
};

}
DS_END_NAMESPACE
