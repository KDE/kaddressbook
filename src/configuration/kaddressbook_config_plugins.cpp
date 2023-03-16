/*
   SPDX-FileCopyrightText: 2016-2023 Laurent Montel <montel@kde.org>

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

#if KCMUTILS_VERSION < QT_VERSION_CHECK(5, 240, 0)
KCMKaddressbookPluginsConfig::KCMKaddressbookPluginsConfig(QWidget *parent, const QVariantList &args)
    : KCModule(parent, args)
    , mConfigurePluginWidget(new PimCommon::ConfigurePluginsWidget(new KAddressBookConfigPluginListWidget(this), this))
#else
KCMKaddressbookPluginsConfig::KCMKaddressbookPluginsConfig(QObject *parent, const KPluginMetaData &data, const QVariantList &args)
    : KCModule(parent, data, args)
    , mConfigurePluginWidget(new PimCommon::ConfigurePluginsWidget(new KAddressBookConfigPluginListWidget(widget()), widget()))
#endif
{
#if KCMUTILS_VERSION < QT_VERSION_CHECK(5, 240, 0)
    auto lay = new QHBoxLayout(this);
#else
    auto lay = new QHBoxLayout(widget());
#endif
    lay->setContentsMargins({});
    connect(mConfigurePluginWidget, &PimCommon::ConfigurePluginsWidget::changed, this, &KCMKaddressbookPluginsConfig::slotConfigChanged);
    lay->addWidget(mConfigurePluginWidget);
}

void KCMKaddressbookPluginsConfig::slotConfigChanged()
{
#if KCMUTILS_VERSION < QT_VERSION_CHECK(5, 240, 0)
    Q_EMIT changed(true);
#else
    markAsChanged();
#endif
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
