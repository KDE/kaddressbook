/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

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
        disconnect(mAccountActivities, &Akonadi::AccountActivitiesAbstract::activitiesChanged, this, &ContactEntityMimeTypeFilterModel::slotInvalidateFilter);
    }
    mAccountActivities = accountActivities;
    if (mAccountActivities) {
        connect(mAccountActivities, &Akonadi::AccountActivitiesAbstract::activitiesChanged, this, &ContactEntityMimeTypeFilterModel::slotInvalidateFilter);
    }
}

void ContactEntityMimeTypeFilterModel::slotInvalidateFilter()
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
    beginFilterChange();
    endFilterChange(QSortFilterProxyModel::Direction::Rows);
#else
    invalidateFilter();
#endif
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
        beginFilterChange();
#endif
        mEnablePlasmaActivities = newEnablePlasmaActivities;
#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
        endFilterChange(QSortFilterProxyModel::Direction::Rows);
#else
        invalidateFilter();
#endif
    }
}

#include "moc_contactentitymimetypefiltermodel.cpp"
