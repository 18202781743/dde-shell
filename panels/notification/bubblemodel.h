// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "dsglobal.h"

#include <QAbstractListModel>
#include <QQmlListProperty>

DS_BEGIN_NAMESPACE
namespace notification {

class BubbleItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text CONSTANT)
    Q_PROPERTY(QString title READ title CONSTANT)
    Q_PROPERTY(QString iconName READ iconName CONSTANT)
    Q_PROPERTY(int level READ level WRITE setLevel NOTIFY levelChanged)
    Q_PROPERTY(bool hasAction READ hasAction CONSTANT)
public:
    explicit BubbleItem();
    explicit BubbleItem(const QString &text, const QString &title, const QString &iconName);

    QString text() const;
    QString title() const;
    QString iconName() const;
    int level() const;
    int id() const;

    void setLevel(int newLevel);
    void setParams(const QString &appName, int id, const QStringList &actions,
                   const QVariantMap hints, int replaceId, const int timeout,
                   const QVariantMap bubbleParams);

    QVariantMap toMap() const;

    bool hasAction() const;
    QString defaultActionText() const;
    QString defaultActionId() const;
    QStringList actionTexts() const;
    QStringList actionIds() const;

Q_SIGNALS:
    void levelChanged();

    void timeout();

public:
    QString m_appName;
    int m_id = 0;
    QStringList m_actions;
    QVariantMap m_hints;
    int m_replaceId;
    int m_timeout = 0;
    QString m_ctime;
    QVariantMap m_extraParams;

private:
    QString m_text;
    QString m_title;
    QString m_iconName;
    int m_level = 0;
};

class BubbleModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum {
        Text = Qt::UserRole + 1,
        Title,
        IconName,
        Level,
        HasAction,
        DefaultActionText,
        DefaultActionId,
        ActionTexts,
        ActionIds,
    } BubbleRule;
    enum {
        Timeout,
        User
    } BubbleReleaseReason;

    explicit BubbleModel(QObject *parent = nullptr);

    void push(BubbleItem *item);
    void clear();
    QList<BubbleItem *> items() const;
    Q_INVOKABLE void remove(int index);
    void remove(BubbleItem *bubble);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    void updateLevel();
    QList<BubbleItem *> m_bubbles;
};

}
DS_END_NAMESPACE
