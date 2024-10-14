/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kaddressbook_config_activities.h"
#include <KAboutData>
#include <KLocalizedString>
#include <KPluginFactory>
#include <PimCommon/ConfigurePluginsListWidget>
#include <PimCommon/ConfigurePluginsWidget>
#include <QHBoxLayout>

using namespace KAddressBook;

K_PLUGIN_CLASS_WITH_JSON(KCMKaddressbookActivitiesConfig, "kaddressbook_config_activities.json")

KCMKaddressbookActivitiesConfig::KCMKaddressbookActivitiesConfig(QObject *parent, const KPluginMetaData &data)
    : KCModule(parent, data)
{
    auto lay = new QHBoxLayout(widget());
    lay->setContentsMargins({});
}

void KCMKaddressbookActivitiesConfig::slotConfigChanged()
{
    markAsChanged();
}

void KCMKaddressbookActivitiesConfig::save()
{
}

void KCMKaddressbookActivitiesConfig::load()
{
}

void KCMKaddressbookActivitiesConfig::defaults()
{
}

#include "kaddressbook_config_activities.moc"
#include "moc_kaddressbook_config_activities.cpp"
