/*
  SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "activitiesmanager.h"
#include "accountactivities.h"
// #include "kaddressbook_activities_debug.h"
#include "ldapactivities.h"

#include <PlasmaActivities/Consumer>

ActivitiesManager::ActivitiesManager(QObject *parent)
    : PimCommonActivities::ActivitiesBaseManager{parent}
    , mLdapActivities(new LdapActivities(this))
    , mAccountActivities(new AccountActivities(this))
{
    connect(this, &ActivitiesManager::activitiesChanged, this, [this]() {
        Q_EMIT mLdapActivities->activitiesChanged();
        Q_EMIT mAccountActivities->activitiesChanged();
    });
}

ActivitiesManager::~ActivitiesManager() = default;

ActivitiesManager *ActivitiesManager::self()
{
    static ActivitiesManager s_self;
    return &s_self;
}

LdapActivities *ActivitiesManager::ldapActivities() const
{
    return mLdapActivities;
}

AccountActivities *ActivitiesManager::accountActivities() const
{
    return mAccountActivities;
}

#include "moc_activitiesmanager.cpp"
