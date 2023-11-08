// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "bubblemodel.h"

#include <QDBusConnection>
#include <QDBusPendingCall>
#include <QDBusReply>
#include <QLoggingCategory>

#include <DDBusSender>

namespace ds {
//DS_BEGIN_NAMESPACE
namespace notification {

BubbleItem::BubbleItem(const QString &title, const QString &text, const QString &iconName)
    : m_title(title)
    , m_text(text)
    , m_iconName(iconName)
{

}

QString BubbleItem::text() const
{
    return m_text;
}

QString BubbleItem::title() const
{
    return m_title;
}

QString BubbleItem::iconName() const
{
    return m_iconName;
}

int BubbleItem::level() const
{
    return m_level;
}

void BubbleItem::setLevel(int newLevel)
{
    if (m_level == newLevel)
        return;
    m_level = newLevel;
    emit levelChanged();
}

BubbleModel::BubbleModel(QObject *parent)
{

}

void BubbleModel::push(BubbleItem *item)
{
    beginInsertRows(QModelIndex(), 0, 0);
    m_bubbles.prepend(item);
    endInsertRows();

    updateLevel();
}

void BubbleModel::clear()
{
    if (m_bubbles.count() <= 0)
        return;
    beginRemoveRows(QModelIndex(), 0, m_bubbles.count() - 1);
    m_bubbles.clear();
    endResetModel();

    updateLevel();
}

QList<BubbleItem *> BubbleModel::items() const
{
    return m_bubbles;
}

void BubbleModel::remove(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    m_bubbles.remove(index);
    endRemoveRows();
    updateLevel();
}

int BubbleModel::rowCount(const QModelIndex &parent) const
{
    return m_bubbles.count();
}

QVariant BubbleModel::data(const QModelIndex &index, int role) const
{
    const int row = index.row();
    if (row >= m_bubbles.size() || !index.isValid())
        return {};

    switch (role) {
    case BubbleModel::Text:
        return m_bubbles[row]->text();
    case BubbleModel::Title:
        return m_bubbles[row]->title();
    case BubbleModel::IconName:
        return m_bubbles[row]->iconName();
    case BubbleModel::Level:
        return m_bubbles[row]->level();
    default:
        break;
    }
    return {};
}

QHash<int, QByteArray> BubbleModel::roleNames() const
{
    QHash<int, QByteArray> mapRoleNames;
    mapRoleNames[BubbleModel::Text] = "text";
    mapRoleNames[BubbleModel::Title] = "title";
    mapRoleNames[BubbleModel::IconName] = "iconName";
    mapRoleNames[BubbleModel::Level] = "level";
    return mapRoleNames;
}

void BubbleModel::updateLevel()
{
    for (int i = 0; i < m_bubbles.count(); i++) {
        auto item = m_bubbles.at(i);
        item->setLevel(i <= 1 ? 1 : i);
    }
    Q_EMIT dataChanged(index(0, 0), index(m_bubbles.count() - 1, 0), {BubbleModel::Level});
}

}
DS_END_NAMESPACE
