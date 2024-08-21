/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "contactentitymimetypefiltermodel.h"

ContactEntityMimeTypeFilterModel::ContactEntityMimeTypeFilterModel(QObject *parent)
    : Akonadi::EntityMimeTypeFilterModel{parent}
{
}

ContactEntityMimeTypeFilterModel::~ContactEntityMimeTypeFilterModel() = default;

void ContactEntityMimeTypeFilterModel::setAccountActivities(Akonadi::AccountActivitiesAbstract *accountActivities)
{
    mAccountActivities = accountActivities;
    // TODO
}

#include "moc_contactentitymimetypefiltermodel.cpp"
