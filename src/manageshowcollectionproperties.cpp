/*
   Copyright (C) 2016-2019 Montel Laurent <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "manageshowcollectionproperties.h"
#include "mainwidget.h"
#include "kaddressbook_debug.h"
#include <PimCommonAkonadi/CollectionAclPage>

#include <AkonadiWidgets/CollectionPropertiesDialog>
#include <AkonadiWidgets/CollectionMaintenancePage>
#include <AkonadiCore/CollectionAttributesSynchronizationJob>
#include <AkonadiCore/CollectionFetchJob>
#include <AkonadiCore/CollectionFetchScope>

#include <KLocalizedString>

ManageShowCollectionProperties::ManageShowCollectionProperties(MainWidget *mainWidget, QObject *parent)
    : QObject(parent)
    , mMainWidget(mainWidget)
{
    static bool pageRegistered = false;

    if (!pageRegistered) {
        Akonadi::CollectionPropertiesDialog::registerPage(new PimCommon::CollectionAclPageFactory);
        Akonadi::CollectionPropertiesDialog::registerPage(new Akonadi::CollectionMaintenancePageFactory);
        pageRegistered = true;
    }
    mPages = QStringList() << QStringLiteral("Akonadi::CollectionGeneralPropertiesPage")
                           << QStringLiteral("Akonadi::CachePolicyPage")
                           << QStringLiteral("PimCommon::CollectionAclPage")
                           << QStringLiteral("Akonadi::CollectionMaintenancePage");
}

ManageShowCollectionProperties::~ManageShowCollectionProperties()
{
}

void ManageShowCollectionProperties::showCollectionProperties()
{
    const Akonadi::Collection col = mMainWidget->currentAddressBook();
    const Akonadi::Collection::Id id = col.id();
    QPointer<Akonadi::CollectionPropertiesDialog> dlg = mHashDialogBox.value(id);
    if (dlg) {
        dlg->activateWindow();
        dlg->raise();
        return;
    }
    Akonadi::CollectionAttributesSynchronizationJob *sync
        = new Akonadi::CollectionAttributesSynchronizationJob(col);
    sync->setProperty("collectionId", id);
    connect(sync, &KJob::result,
            this, &ManageShowCollectionProperties::slotCollectionPropertiesContinued);
    sync->start();
}

void ManageShowCollectionProperties::slotCollectionPropertiesContinued(KJob *job)
{
    if (job) {
        Akonadi::CollectionAttributesSynchronizationJob *sync
            = qobject_cast<Akonadi::CollectionAttributesSynchronizationJob *>(job);
        Q_ASSERT(sync);
        if (sync->property("collectionId") != mMainWidget->currentAddressBook().id()) {
            return;
        }
    }
    Akonadi::CollectionFetchJob *fetch = new Akonadi::CollectionFetchJob(mMainWidget->currentAddressBook(),
                                                                         Akonadi::CollectionFetchJob::Base);
    fetch->fetchScope().setIncludeStatistics(true);
    connect(fetch, &KJob::result,
            this, &ManageShowCollectionProperties::slotCollectionPropertiesFinished);
}

void ManageShowCollectionProperties::slotCollectionPropertiesFinished(KJob *job)
{
    if (!job) {
        return;
    }

    Akonadi::CollectionFetchJob *fetch = qobject_cast<Akonadi::CollectionFetchJob *>(job);
    Q_ASSERT(fetch);
    if (fetch->collections().isEmpty()) {
        qCWarning(KADDRESSBOOK_LOG) << "no collection";
        return;
    }

    const Akonadi::Collection collection = fetch->collections().first();

    QPointer<Akonadi::CollectionPropertiesDialog> dlg = new Akonadi::CollectionPropertiesDialog(collection, mPages, mMainWidget);
    dlg->setWindowTitle(i18nc("@title:window", "Properties of Address Book Folder %1", collection.name()));

    dlg->show();
    mHashDialogBox.insert(collection.id(), dlg);
}
