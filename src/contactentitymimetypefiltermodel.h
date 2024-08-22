/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <Akonadi/EntityMimeTypeFilterModel>
#include <QObject>
namespace Akonadi
{
class AccountActivitiesAbstract;
}
class ContactEntityMimeTypeFilterModel : public Akonadi::EntityMimeTypeFilterModel
{
    Q_OBJECT
public:
    explicit ContactEntityMimeTypeFilterModel(QObject *parent = nullptr);
    ~ContactEntityMimeTypeFilterModel() override;

    void setAccountActivities(Akonadi::AccountActivitiesAbstract *account);

protected:
    [[nodiscard]] bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    Akonadi::AccountActivitiesAbstract *mAccountActivities = nullptr;
};
