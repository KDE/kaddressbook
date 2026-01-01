/*
  SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once
#include "kaddressbook_export.h"
#include <PimCommonActivities/ActivitiesBaseManager>
#include <QObject>
class LdapActivities;
class AccountActivities;
class KADDRESSBOOK_EXPORT ActivitiesManager : public PimCommonActivities::ActivitiesBaseManager
{
    Q_OBJECT
public:
    static ActivitiesManager *self();

    explicit ActivitiesManager(QObject *parent = nullptr);
    ~ActivitiesManager() override;

    [[nodiscard]] LdapActivities *ldapActivities() const;

    AccountActivities *accountActivities() const;

private:
    LdapActivities *const mLdapActivities;
    AccountActivities *const mAccountActivities;
};
