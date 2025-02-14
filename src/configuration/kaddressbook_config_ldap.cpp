/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kaddressbook_config_ldap.h"
#include <KLDAPWidgets/LdapConfigureWidgetNg>
#include <KPluginFactory>
#include <QCheckBox>
#include <QVBoxLayout>
using namespace KAddressBook;
K_PLUGIN_CLASS_WITH_JSON(KCMKaddressbookLdapConfig, "kaddressbook_config_ldap.json")

KCMKaddressbookLdapConfig::KCMKaddressbookLdapConfig(QObject *parent, const KPluginMetaData &data)
    : KCModule(parent, data)
    , mLdapConfigureWidget(new KLDAPWidgets::LdapConfigureWidgetNg(widget()))
{
    auto lay = new QVBoxLayout(widget());
    lay->setContentsMargins({});
    lay->addWidget(mLdapConfigureWidget);
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
