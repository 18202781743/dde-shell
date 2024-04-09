// SPDX-FileCopyrightText: 2018 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "thememanager.h"
#include "utils.h"

#include <DApplication>
#include <DPlatformTheme>
#include <DLineEdit>

DGUI_USE_NAMESPACE

ThemeManager::ThemeManager(QObject *parent)
    : QObject(parent)
    , m_themeType(UnknownType)
{
    appThemeTypeChanged(Dtk::Gui::DGuiApplicationHelper::instance()->themeType());
    // 主题发生变化触发的信号
    connect(Dtk::Gui::DGuiApplicationHelper::instance(), &Dtk::Gui::DGuiApplicationHelper::themeTypeChanged, this, &ThemeManager::appThemeTypeChanged);
}

ThemeManager::~ThemeManager()
{
}

QString ThemeManager::getIcon(QString path)
{
    if (path.startsWith("common")) {
        return ":/" + path;
    }
    switch (m_themeType) {
    case GreeterType:
    case LockType:
    case DarkType:
        return ":/dark/" + path;
    default:
        return ":/light/" + path;
    }
}

void ThemeManager::setThemeType(ThemeType type)
{
    if (type != m_themeType) {
        m_themeType = type;
        emit themeChanged(m_themeType);
    }
}

void ThemeManager::updateInputStyle(Dtk::Widget::DLineEdit *inputEdit)
{
    switch (m_themeType) {
    case GreeterType:
    case LockType: {
        QPalette editPalette = inputEdit->palette();
        editPalette.setColor(QPalette::Button, QColor(255, 255, 255, static_cast<int>(0.2 * 255)));
        inputEdit->setPalette(editPalette);
    } break;
    case DarkType: {
        QPalette editPalette = inputEdit->palette();
        editPalette.setColor(QPalette::Button, QColor(255, 255, 255, static_cast<int>(0.2 * 255)));
        inputEdit->setPalette(editPalette);
    } break;
    case LightType: {
        QPalette editPalette = inputEdit->palette();
        editPalette.setColor(QPalette::Button, QColor(0, 0, 0, static_cast<int>(0.2 * 255)));
        inputEdit->setPalette(editPalette);
    } break;
    default:
        break;
    }
}

void ThemeManager::appThemeTypeChanged(Dtk::Gui::DGuiApplicationHelper::ColorType themeType)
{
    if (m_themeType >= UnknownType) {
        switch (themeType) {
        case Dtk::Gui::DGuiApplicationHelper::ColorType::DarkType:
            setThemeType(DarkType);
            break;
        case Dtk::Gui::DGuiApplicationHelper::ColorType::LightType:
            setThemeType(LightType);
            break;
        default:
            break;
        }
    }
}

QColor ThemeManager::backgroundColor()
{
    int alpha = Utils::isWaylandEnvironment() ? static_cast<int>(0.5 * 255) : static_cast<int>(0.03 * 255);
    switch (m_themeType) {
    case DarkType:
        return QColor(0, 0, 0, alpha);
    case GreeterType:
    case LockType:
        return QColor(235, 235, 235, static_cast<int>(0.05 * 255));
    default:
        return QColor(255, 255, 255, alpha);
    }
    Q_UNREACHABLE();
    return QColor(255, 255, 255, alpha);
}

QColor ThemeManager::lineColor()
{
    switch (m_themeType) {
    case DarkType:
        return QColor(255, 255, 255, static_cast<int>(0.05 * 255));
    case LockType:
        return QColor(0, 0, 0, static_cast<int>(0.1 * 255));
    default:
        return QColor(0, 0, 0, static_cast<int>(0.1 * 255));
    }
    Q_UNREACHABLE();
    return QColor(255, 255, 255, static_cast<int>(0.03 * 255));
}

QColor ThemeManager::itemBackgroundColor()
{
    switch (m_themeType) {
    case DarkType:
        return QColor(255, 255, 255, static_cast<int>(255 * 0.10));
    case GreeterType:
    case LockType:
        return QColor(255, 255, 255, static_cast<int>(255 * 0.10));
    default:
        return QColor(0, 0, 0, static_cast<int>(255 * 0.10));
    }
    Q_UNREACHABLE();
    return QColor(255, 255, 255, static_cast<int>(0.03 * 255));
}
