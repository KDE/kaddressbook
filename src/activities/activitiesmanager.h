/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once
#include "kaddressbook_export.h"
#include <QObject>
namespace KActivities
{
class Consumer;
}
class TransportActivities;
class IdentityActivities;
class LdapActivities;
class KADDRESSBOOK_EXPORT ActivitiesManager : public QObject
{
    Q_OBJECT
public:
    static ActivitiesManager *self();

    explicit ActivitiesManager(QObject *parent = nullptr);
    ~ActivitiesManager() override;

    [[nodiscard]] bool enabled() const;
    void setEnabled(bool newEnabled);

    [[nodiscard]] bool isInCurrentActivity(const QStringList &lst) const;
    [[nodiscard]] QString currentActivity() const;
    [[nodiscard]] LdapActivities *ldapActivities() const;

Q_SIGNALS:
    void activitiesChanged();

private:
    LdapActivities *const mLdapActivities;
    KActivities::Consumer *const mActivitiesConsumer;
    bool mEnabled = false;
};
