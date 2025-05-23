/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kaddressbook_config_ldap.h"
#include "config-kaddressbook.h"
#include "settings.h"
#include <KLDAPWidgets/LdapConfigureWidgetNg>
#include <KPluginFactory>
#include <QCheckBox>
#include <QVBoxLayout>

#if HAVE_ACTIVITY_SUPPORT
#include "activities/activitiesmanager.h"
#include "activities/ldapactivities.h"
#endif

using namespace KAddressBook;
K_PLUGIN_CLASS_WITH_JSON(KCMKaddressbookLdapConfig, "kaddressbook_config_ldap.json")

KCMKaddressbookLdapConfig::KCMKaddressbookLdapConfig(QObject *parent, const KPluginMetaData &data)
    : KCModule(parent, data)
    , mLdapConfigureWidget(new KLDAPWidgets::LdapConfigureWidgetNg(widget()))
{
    auto lay = new QVBoxLayout(widget());
    lay->setContentsMargins({});
    lay->addWidget(mLdapConfigureWidget);
#if HAVE_ACTIVITY_SUPPORT
    mLdapConfigureWidget->setEnablePlasmaActivities(Settings::self()->enabledActivities());
    mLdapConfigureWidget->setLdapActivitiesAbstract(ActivitiesManager::self()->ldapActivities());
#endif
}

/*
void KCMKaddressbookLdapConfig::slotConfigChanged()
{
    markAsChanged();
}
*/

void KCMKaddressbookLdapConfig::save()
{
    mLdapConfigureWidget->save();
}

void KCMKaddressbookLdapConfig::load()
{
    mLdapConfigureWidget->load();
}

void KCMKaddressbookLdapConfig::defaults()
{
}

#include "kaddressbook_config_ldap.moc"
#include "moc_kaddressbook_config_ldap.cpp"
