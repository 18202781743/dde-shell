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
public:
    explicit BubbleItem(const QString &title, const QString &text, const QString &iconName);

    QString text() const;
    QString title() const;
    QString iconName() const;
    int level() const;

    void setLevel(int newLevel);

Q_SIGNALS:
    void levelChanged();

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
        Level
    } BubbleRule;

    explicit BubbleModel(QObject *parent = nullptr);

    void push(BubbleItem *item);
    void clear();
    QList<BubbleItem *> items() const;
    Q_INVOKABLE void remove(int index);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    void updateLevel();
    QList<BubbleItem *> m_bubbles;
};

}
DS_END_NAMESPACE
