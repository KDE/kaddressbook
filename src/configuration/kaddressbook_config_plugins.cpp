/*
   SPDX-FileCopyrightText: 2016-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kaddressbook_config_plugins.h"
#include "kaddressbookconfigpluginlistwidget.h"
#include <KAboutData>
#include <KLocalizedString>
#include <KPluginFactory>
#include <PimCommon/ConfigurePluginsListWidget>
#include <PimCommon/ConfigurePluginsWidget>
#include <QHBoxLayout>

using namespace KAddressBook;

K_PLUGIN_CLASS_WITH_JSON(KCMKaddressbookPluginsConfig, "kaddressbook_config_plugins.json")

KCMKaddressbookPluginsConfig::KCMKaddressbookPluginsConfig(QObject *parent, const KPluginMetaData &data)
    : KCModule(parent, data)
    , mConfigurePluginWidget(new PimCommon::ConfigurePluginsWidget(new KAddressBookConfigPluginListWidget(widget()), widget()))
{
    auto lay = new QHBoxLayout(widget());
    lay->setContentsMargins({});
    connect(mConfigurePluginWidget, &PimCommon::ConfigurePluginsWidget::changed, this, &KCMKaddressbookPluginsConfig::slotConfigChanged);
    lay->addWidget(mConfigurePluginWidget);
}

void KCMKaddressbookPluginsConfig::slotConfigChanged()
{
    markAsChanged();
}

void KCMKaddressbookPluginsConfig::save()
{
    mConfigurePluginWidget->save();
}

void KCMKaddressbookPluginsConfig::load()
{
    mConfigurePluginWidget->doLoadFromGlobalSettings();
}

void KCMKaddressbookPluginsConfig::defaults()
{
    mConfigurePluginWidget->doResetToDefaultsOther();
}

#include "kaddressbook_config_plugins.moc"

#include "moc_kaddressbook_config_plugins.cpp"
