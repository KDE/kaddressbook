/*
  SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <KLDAPCore/LdapActivitiesAbstract>
#include <PimCommonActivities/ActivitiesBaseManager>
class ActivitiesManager;
class LdapActivities : public PimCommonActivities::ActivitiesFilter<KLDAPCore::LdapActivitiesAbstract>
{
    Q_OBJECT
public:
    explicit LdapActivities(ActivitiesManager *manager);
    ~LdapActivities() override;
};
