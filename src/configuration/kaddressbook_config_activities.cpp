/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kaddressbook_config_activities.h"
#include <KLocalizedString>
#include <KPluginFactory>
#include <QCheckBox>
#include <QVBoxLayout>

using namespace KAddressBook;

K_PLUGIN_CLASS_WITH_JSON(KCMKaddressbookActivitiesConfig, "kaddressbook_config_activities.json")

KCMKaddressbookActivitiesConfig::KCMKaddressbookActivitiesConfig(QObject *parent, const KPluginMetaData &data)
    : KCModule(parent, data)
    , mActivities(new QCheckBox(i18n("Enabled"), widget()))
{
    auto lay = new QVBoxLayout(widget());
    lay->setContentsMargins({});
    lay->addWidget(mActivities);
    lay->addStretch(1);
    connect(mActivities, &QCheckBox::clicked, this, &KCMKaddressbookActivitiesConfig::slotConfigChanged);
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
