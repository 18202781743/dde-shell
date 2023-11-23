// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "exampleapplet.h"

#include "pluginfactory.h"
#include "appletitem.h"
#include <DConfig>

ExampleApplet::ExampleApplet(QObject *parent)
    : DApplet(parent)
    , m_mainText("Custom Applet")
{

}

QString ExampleApplet::mainText() const
{
    return m_mainText;
}

bool ExampleApplet::load(const DAppletData &)
{
    DCORE_USE_NAMESPACE;
    std::unique_ptr<DConfig> config(DConfig::create("org.deepin.ds.example", "org.deepin.ds.example"));
    return config->value("loadAppletExampleData").toBool();
}

bool ExampleApplet::init()
{
    DApplet::init();

    DAppletItem *root = qobject_cast<DAppletItem *>(rootObject());
    Q_ASSERT(root);

    m_mainText = QString("%1 - w:%2;h:%3").arg(m_mainText).arg(root->width()).arg(root->height());
    Q_EMIT mainTextChanged();
    return true;
}

D_APPLET_CLASS(ExampleApplet)

#include "exampleapplet.moc"
