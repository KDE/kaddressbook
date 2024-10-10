/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "contactentitymimetypefiltermodel.h"
#include <Akonadi/AccountActivitiesAbstract>
#include <Akonadi/AgentInstance>
#include <Akonadi/AgentManager>

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

bool ContactEntityMimeTypeFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (mAccountActivities && mEnablePlasmaActivities) {
        const QModelIndex modelIndex = sourceModel()->index(sourceRow, 0, sourceParent);

        const auto collection = sourceModel()->data(modelIndex, Akonadi::EntityTreeModel::CollectionRole).value<Akonadi::Collection>();
        const Akonadi::AgentInstance instance = Akonadi::AgentManager::self()->instance(collection.resource());
        if (instance.activitiesEnabled()) {
            return mAccountActivities->filterAcceptsRow(instance.activities());
        }
    }
    return Akonadi::EntityMimeTypeFilterModel::filterAcceptsRow(sourceRow, sourceParent);
}

Akonadi::AccountActivitiesAbstract *ContactEntityMimeTypeFilterModel::accountActivities() const
{
    return mAccountActivities;
}

bool ContactEntityMimeTypeFilterModel::enablePlasmaActivities() const
{
    return mEnablePlasmaActivities;
}

void ContactEntityMimeTypeFilterModel::setEnablePlasmaActivities(bool newEnablePlasmaActivities)
{
    if (mEnablePlasmaActivities != newEnablePlasmaActivities) {
        mEnablePlasmaActivities = newEnablePlasmaActivities;
        invalidateFilter();
    }
}

#include "moc_contactentitymimetypefiltermodel.cpp"
