// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "bubblemodel.h"

#include <QDBusConnection>
#include <QDBusPendingCall>
#include <QDBusReply>
#include <QTimer>
#include <QLoggingCategory>
#include <QDateTime>

#include <DDBusSender>

namespace ds {
//DS_BEGIN_NAMESPACE
namespace notification {

BubbleItem::BubbleItem()
{

}

BubbleItem::BubbleItem(const QString &text, const QString &title, const QString &iconName)
    : m_text(text)
    , m_title(title)
    , m_iconName(iconName)
    , m_ctime(QString::number(QDateTime::currentMSecsSinceEpoch()))
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

int BubbleItem::id() const
{
    return m_id;
}

void BubbleItem::setLevel(int newLevel)
{
    if (m_level == newLevel)
        return;
    m_level = newLevel;
    emit levelChanged();
}

void BubbleItem::setParams(const QString &appName, int id, const QStringList &actions, const QVariantMap hints, int replaceId, const int timeout, const QVariantMap bubbleParams)
{
    m_appName = appName;
    m_id = id;
    m_actions = actions;
    m_hints = hints;
    m_replaceId = replaceId;
    m_timeout = timeout;
    m_extraParams = bubbleParams;
    if (m_timeout > 0) {
        auto timer = new QTimer(this);
        timer->setSingleShot(true);
        timer->setInterval(m_timeout);
        QObject::connect(timer, &QTimer::timeout, this, &BubbleItem::timeout);
        timer->start();
    }
}

QVariantMap BubbleItem::toMap() const
{
    QVariantMap res;
    res["id"] = m_id;
    res["replaceId"] = m_replaceId;
    res["appName"] = m_appName;
    res["appIcon"] = m_iconName;
    res["summary"] = m_title;
    res["body"] = m_text;
    res["actions"] = m_actions;
    res["hints"] = m_hints;
    res["ctime"] = m_hints;
    res["extraParams"] = m_extraParams;
    return res;
}

QString BubbleItem::defaultActionText() const
{
    const auto index = defaultActionTextIndex();
    if (index < 0)
        return QString();
    return m_actions[index];
}

QString BubbleItem::defaultActionId() const
{
    const auto index = defaultActionIdIndex();
    if (index < 0)
        return QString();
    return m_actions[index];
}

QString BubbleItem::firstActionText() const
{
    if (!hasDisplayAction())
        return QString();
    return displayActions()[1];
}

QString BubbleItem::firstActionId() const
{
    if (!hasDisplayAction())
        return QString();
    return displayActions()[0];
}

QStringList BubbleItem::actionTexts() const
{
    QStringList res;
    const auto tmp = displayActions();
    for (int i = 3; i < tmp.count(); i += 2)
        res << tmp[i];
    return res;
}

QStringList BubbleItem::actionIds() const
{
    QStringList res;
    const auto tmp = displayActions();
    for (int i = 2; i < tmp.count(); i += 2)
        res << tmp[i];
    return res;
}

int BubbleItem::defaultActionIdIndex() const
{
    return m_actions.indexOf("default");
}

int BubbleItem::defaultActionTextIndex() const
{
    const auto index = defaultActionTextIndex();
    if (index >= 0)
        return index + 1;
    return -1;
}

QStringList BubbleItem::displayActions() const
{
    const auto defaultIndex = defaultActionIdIndex();
    if (defaultIndex >= 0) {
        auto tmp = m_actions;
        tmp.remove(defaultIndex, 2);
        return tmp;
    }
    return m_actions;
}

bool BubbleItem::hasDisplayAction() const
{
    const auto tmp = displayActions();
    return tmp.count() >= 2;
}

bool BubbleItem::hasDefaultAction() const
{
    return defaultActionIdIndex() >= 0;
}

BubbleModel::BubbleModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

void BubbleModel::push(BubbleItem *item)
{
//    if (m_bubbles.count() >= 5) {
//        remove(m_bubbles.count() - 1);
//    }
    bool more = displayRowCount() >= BubbleMaxCount;
    if (more) {
        beginRemoveRows(QModelIndex(), BubbleMaxCount - 1, BubbleMaxCount - 1);
        endRemoveRows();
    }
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
    if (index < 0 || index >= displayRowCount())
        return;

    beginRemoveRows(QModelIndex(), index, index);
    auto bubble = m_bubbles.takeAt(index);
    bubble->deleteLater();
    endRemoveRows();

    if (m_bubbles.count() >= BubbleMaxCount) {
        beginInsertRows(QModelIndex(), displayRowCount() - 1, displayRowCount() - 1);
        endInsertRows();
        updateLevel();
    }
}

void BubbleModel::remove(BubbleItem *bubble)
{
    const auto index = m_bubbles.indexOf(bubble);
    if (index >= 0) {
        remove(index);
    }
}

int BubbleModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return displayRowCount();
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
    case BubbleModel::OverlayCount:
        return overlayCount();
    case BubbleModel::hasDefaultAction:
        return m_bubbles[row]->hasDefaultAction();
    case BubbleModel::hasDisplayAction:
        return m_bubbles[row]->hasDisplayAction();
    case BubbleModel::FirstActionText:
        return m_bubbles[row]->firstActionText();
    case BubbleModel::FirstActionId:
        return m_bubbles[row]->firstActionId();
    case BubbleModel::ActionTexts:
        return m_bubbles[row]->actionTexts();
    case BubbleModel::ActionIds:
        return m_bubbles[row]->actionIds();
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
    mapRoleNames[BubbleModel::OverlayCount] = "overlayCount";
    mapRoleNames[BubbleModel::hasDefaultAction] = "hasDefaultAction";
    mapRoleNames[BubbleModel::hasDisplayAction] = "hasDisplayAction";
    mapRoleNames[BubbleModel::FirstActionText] = "firstActionText";
    mapRoleNames[BubbleModel::FirstActionId] = "firstActionId";
    mapRoleNames[BubbleModel::ActionTexts] = "actionTexts";
    mapRoleNames[BubbleModel::ActionIds] = "actionIds";
    return mapRoleNames;
}

int BubbleModel::displayRowCount() const
{
    return qMin(m_bubbles.count(), BubbleMaxCount);
}

int BubbleModel::overlayCount() const
{
    return qMin(m_bubbles.count() - displayRowCount(), OverlayMaxCount);
}

void BubbleModel::updateLevel()
{
    if (m_bubbles.isEmpty())
        return;

    for (int i = 0; i < displayRowCount(); i++) {
        auto item = m_bubbles.at(i);
        item->setLevel(i <= 1 ? 1 : 1 + overlayCount());
    }
    Q_EMIT dataChanged(index(0), index(displayRowCount() - 1), {BubbleModel::Level});
}

}
DS_END_NAMESPACE
