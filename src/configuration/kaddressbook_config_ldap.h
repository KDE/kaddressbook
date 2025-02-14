/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <KCModule>

#include <QVariant>
namespace KLDAPWidgets
{
class LdapConfigureWidgetNg;
}

namespace KAddressBook
{
class KCMKaddressbookLdapConfig : public KCModule
{
    Q_OBJECT
public:
    explicit KCMKaddressbookLdapConfig(QObject *parent, const KPluginMetaData &data);
    void load() override;
    void save() override;
    void defaults() override;

private:
    KLDAPWidgets::LdapConfigureWidgetNg *const mLdapConfigureWidget;
};
}
