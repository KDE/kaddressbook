/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "contactentitymimetypefiltermodel.h"
#include <Akonadi/AccountActivitiesAbstract>

ContactEntityMimeTypeFilterModel::ContactEntityMimeTypeFilterModel(QObject *parent)
    : Akonadi::EntityMimeTypeFilterModel{parent}
{
}

ContactEntityMimeTypeFilterModel::~ContactEntityMimeTypeFilterModel() = default;

void ContactEntityMimeTypeFilterModel::setAccountActivities(Akonadi::AccountActivitiesAbstract *accountActivities)
{
    if (mAccountActivities) {
        disconnect(mAccountActivities, &Akonadi::AccountActivitiesAbstract::activitiesChanged, this, &ContactEntityMimeTypeFilterModel::invalidateFilter);
    }
    mAccountActivities = accountActivities;
    if (mAccountActivities) {
        connect(mAccountActivities, &Akonadi::AccountActivitiesAbstract::activitiesChanged, this, &ContactEntityMimeTypeFilterModel::invalidateFilter);
    }
}

#include "moc_contactentitymimetypefiltermodel.cpp"
