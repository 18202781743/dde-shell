// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "applet.h"

DS_BEGIN_NAMESPACE
namespace notification {

class BubbleApplet : public DApplet
{
    Q_OBJECT
  public:
    explicit BubbleApplet(QObject *parent = nullptr);
    virtual void init() override;

};

}
DS_END_NAMESPACE
