/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once
#include "kaddressbook_export.h"
#include <PimCommonActivities/ActivitiesBaseManager>
#include <QObject>
namespace KActivities
{
class Consumer;
}
class LdapActivities;
class AccountActivities;
class KADDRESSBOOK_EXPORT ActivitiesManager : public PimCommonActivities::ActivitiesBaseManager
{
    Q_OBJECT
public:
    static ActivitiesManager *self();

    explicit ActivitiesManager(QObject *parent = nullptr);
    ~ActivitiesManager() override;

    [[nodiscard]] bool enabled() const override;
    void setEnabled(bool newEnabled);

    [[nodiscard]] LdapActivities *ldapActivities() const;

    AccountActivities *accountActivities() const;

private:
    LdapActivities *const mLdapActivities;
    KActivities::Consumer *const mActivitiesConsumer;
    AccountActivities *const mAccountActivities;
    bool mEnabled = false;
};
