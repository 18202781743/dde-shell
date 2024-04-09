/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was:
 *
 * qdbusxml2cpp is Copyright (C) 2016 The Qt Company Ltd.
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef DHCP6CONFIGINTERFACE_H
#define DHCP6CONFIGINTERFACE_H

#include "generictypes.h"

#include <QObject>
#include <QString>
#include <QVariant>
#include <QtDBus>

/*
 * Proxy class for interface org.freedesktop.NetworkManager.DHCP6Config
 */
class OrgFreedesktopNetworkManagerDHCP6ConfigInterface : public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
#ifdef NMQT_STATIC
    {
        return "org.kde.fakenetwork.DHCP6Config";
    }
#else
    {
        return "org.freedesktop.NetworkManager.DHCP6Config";
    }
#endif

public:
    OrgFreedesktopNetworkManagerDHCP6ConfigInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = nullptr);

    ~OrgFreedesktopNetworkManagerDHCP6ConfigInterface() override;

    Q_PROPERTY(QVariantMap Options READ options)
    inline QVariantMap options() const
    {
        return qvariant_cast<QVariantMap>(property("Options"));
    }

public Q_SLOTS: // METHODS
Q_SIGNALS: // SIGNALS
    void PropertiesChanged(const QVariantMap &properties);
};

#endif
