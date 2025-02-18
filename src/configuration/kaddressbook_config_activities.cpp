/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kaddressbook_config_activities.h"
#include "settings.h"
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
    Settings::self()->setEnabledActivities(mActivities->isChecked());
    setNeedsSave(false);
}

void KCMKaddressbookActivitiesConfig::load()
{
    mActivities->setChecked(Settings::self()->enabledActivities());
}

void KCMKaddressbookActivitiesConfig::defaults()
{
    mActivities->setChecked(false);
}

#include "kaddressbook_config_activities.moc"
#include "moc_kaddressbook_config_activities.cpp"
