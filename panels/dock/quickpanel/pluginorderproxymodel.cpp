// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pluginorderproxymodel.h"

#include <QDebug>
#include <DConfig>
DCORE_USE_NAMESPACE

namespace dock {

PluginOrderProxyModel::PluginOrderProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    updateQuickPluginsOrder();
    sort(0);
}

QString PluginOrderProxyModel::getTitle(const QString &pluginName)
{
    return surfaceValue(pluginName, "title").toString();
}

QColor PluginOrderProxyModel::getSurfaceItem(const QString &pluginName)
{
    return surfaceValue(pluginName, "color").value<QColor>();
}

bool PluginOrderProxyModel::lessThan(const QModelIndex &sourceLeft, const QModelIndex &sourceRight) const
{
    auto leftOrder = pluginOrder(sourceLeft);
    auto rightOrder = pluginOrder(sourceRight);

    return leftOrder < rightOrder;
}

bool PluginOrderProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    const auto index = this->sourceModel()->index(sourceRow, 0, sourceParent);
    if (!index.isValid())
        return false;
    const auto &name = surfaceName(index);
    return !m_hideInPanelPlugins.contains(name);
}

void PluginOrderProxyModel::updateQuickPluginsOrder()
{
    QScopedPointer<DConfig> dconfig(DConfig::create("org.deepin.ds.dock", "org.deepin.ds.dock.quick-panel"));
    m_quickPlugins = dconfig->value("quickPluginsOrder").toStringList();
    m_hideInPanelPlugins = dconfig->value("hiddenQuickPlugins").toStringList();
    qDebug() << "Fetched QuickPanel's orders by DConfig,"
             << "order list size:" << m_quickPlugins.size()
             << "hide size:" <<m_hideInPanelPlugins.size();
    invalidate();
}

int PluginOrderProxyModel::pluginOrder(const QModelIndex &index) const
{
    const auto name = surfaceName(index);
    auto ret = m_quickPlugins.indexOf(name);
    auto order = surfaceOrder(index);
    if (order >= 0) {
        ret = order;
    }
    auto type = surfaceType(index);
    const QMap<int, int> OrderOffset {
        {1, 2000},
        {2, 1000},
        {4, 4000},
    };
    ret += OrderOffset.value(type);

    return ret;
}

int PluginOrderProxyModel::surfaceType(const QModelIndex &index) const
{
    return surfaceValue(index, "type").toInt();
}

int PluginOrderProxyModel::surfaceOrder(const QModelIndex &index) const
{
    return surfaceValue(index, "order").toInt();
}

QColor PluginOrderProxyModel::surfaceColor(const QModelIndex &index)
{
    return surfaceValue(index, "color").value<QColor>();
}

QString PluginOrderProxyModel::surfaceName(const QModelIndex &index) const
{
    return surfaceValue(index, "pluginName").toString();
}

QVariant PluginOrderProxyModel::surfaceValue(const QModelIndex &index, const QByteArray &roleName) const
{
    const auto role = roleByName(roleName);
    if (role >= 0)
        return surfaceModel()->data(index, role);

    return {};
}

QVariant PluginOrderProxyModel::surfaceValue(const QString &pluginName, const QByteArray &roleName) const
{
    const auto targetModel = surfaceModel();
    for (int i = 0; i < targetModel->rowCount(); i++) {
        const auto index = targetModel->index(i, 0);
        const auto name = surfaceName(index);
        if (name == pluginName)
            return surfaceValue(index, roleName);
    }
    return {};
}

int PluginOrderProxyModel::roleByName(const QByteArray &roleName) const
{
    if (!surfaceModel())
        return -1;
    const auto roleNames = surfaceModel()->roleNames();
    return roleNames.key(roleName, -1);
}

QAbstractListModel *PluginOrderProxyModel::surfaceModel() const
{
    return qobject_cast<QAbstractListModel *>(sourceModel());
}

}
