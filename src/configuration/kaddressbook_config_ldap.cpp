/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kaddressbook_config_ldap.h"
#include <KPluginFactory>
#include <QCheckBox>
#include <QVBoxLayout>
using namespace KAddressBook;
K_PLUGIN_CLASS_WITH_JSON(KCMKaddressbookLdapConfig, "kaddressbook_config_ldap.json")

KCMKaddressbookLdapConfig::KCMKaddressbookLdapConfig(QObject *parent, const KPluginMetaData &data)
    : KCModule(parent, data)
{
    auto lay = new QVBoxLayout(widget());
    lay->setContentsMargins({});
    // lay->addWidget(mActivities);
    // lay->addStretch(1);
}

/*
void KCMKaddressbookLdapConfig::slotConfigChanged()
{
    markAsChanged();
}
*/

void KCMKaddressbookLdapConfig::save()
{
    // Settings::self()->setEnabledActivities(mActivities->isChecked());
}

void KCMKaddressbookLdapConfig::load()
{
    // mActivities->setChecked(Settings::self()->enabledActivities());
}

void KCMKaddressbookLdapConfig::defaults()
{
    // mActivities->setChecked(false);
}

#include "kaddressbook_config_ldap.moc"
#include "moc_kaddressbook_config_ldap.cpp"
