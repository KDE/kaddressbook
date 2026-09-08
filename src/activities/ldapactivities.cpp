/*
  SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "ldapactivities.h"
#include "activitiesmanager.h"

LdapActivities::LdapActivities(ActivitiesManager *manager)
    : PimCommonActivities::ActivitiesFilter<KLDAPCore::LdapActivitiesAbstract>{manager}
{
}

LdapActivities::~LdapActivities() = default;

#include "moc_ldapactivities.cpp"
