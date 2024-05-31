// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QtQml/qqml.h>
#include <QSortFilterProxyModel>
#include <QColor>

namespace dock {

class PluginOrderProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    QML_NAMED_ELEMENT(PluginOrderProxyModel)
public:
    explicit PluginOrderProxyModel(QObject *parent = nullptr);

    Q_INVOKABLE QString getTitle(const QString &pluginName);
    Q_INVOKABLE QColor getSurfaceItem(const QString &pluginName);

    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

protected:
    bool lessThan(const QModelIndex &sourceLeft, const QModelIndex &sourceRight) const override;
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    void updateQuickPluginsOrder();
    int pluginOrder(const QModelIndex &index) const;
    int surfaceType(const QModelIndex &index) const;
    int surfaceOrder(const QModelIndex &index) const;
    QColor surfaceColor(const QModelIndex &index);
    QString surfaceName(const QModelIndex &index) const;
    QVariant surfaceValue(const QModelIndex &index, const QByteArray &roleName) const;
    QVariant surfaceValue(const QString &pluginName, const QByteArray &roleName) const;
    QObject *surfaceObject(const QModelIndex &index) const;
    int roleByName(const QByteArray &roleName) const;
    QAbstractListModel *surfaceModel() const;

private:
    QStringList m_quickPlugins;
    QStringList m_hideInPanelPlugins;
};

}
