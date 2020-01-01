/*
   Copyright (C) 2016-2020 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "kaddressbook_config_plugins.h"
#include "kaddressbookconfigpluginlistwidget.h"
#include <KAboutData>
#include <KPluginFactory>
#include <KLocalizedString>
#include <PimCommon/ConfigurePluginsWidget>
#include <PimCommon/ConfigurePluginsListWidget>
#include <QHBoxLayout>

using namespace KAddressBook;

K_PLUGIN_FACTORY(KCMKaddressbookPluginsConfigFactory, registerPlugin<KCMKaddressbookPluginsConfig>();
                 )

KCMKaddressbookPluginsConfig::KCMKaddressbookPluginsConfig(QWidget *parent, const QVariantList &args)
    : KCModule(parent, args)
{
    QHBoxLayout *lay = new QHBoxLayout(this);
    lay->setContentsMargins(0, 0, 0, 0);

    KAboutData *about = new KAboutData(QStringLiteral("kcmakrpluginsconfig"),
                                       i18n("Configure Plugins"),
                                       QString(), QString(), KAboutLicense::GPL,
                                       i18n("(c), 2016-2019 Laurent Montel"));

    about->addAuthor(i18n("Laurent Montel"), QString(), QStringLiteral("montel@kde.org"));

    setAboutData(about);
    mConfigurePluginWidget = new PimCommon::ConfigurePluginsWidget(new KAddressBookConfigPluginListWidget(this), this);
    connect(mConfigurePluginWidget, &PimCommon::ConfigurePluginsWidget::changed, this, &KCMKaddressbookPluginsConfig::slotConfigChanged);
    lay->addWidget(mConfigurePluginWidget);
}

void KCMKaddressbookPluginsConfig::slotConfigChanged()
{
    Q_EMIT changed(true);
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
