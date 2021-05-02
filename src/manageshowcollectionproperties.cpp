/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "manageshowcollectionproperties.h"
#include "kaddressbook_debug.h"
#include "mainwidget.h"
#include <PimCommonAkonadi/CollectionAclPage>

#include <AkonadiCore/CollectionAttributesSynchronizationJob>
#include <AkonadiCore/CollectionFetchJob>
#include <AkonadiCore/CollectionFetchScope>
#include <AkonadiWidgets/CollectionMaintenancePage>
#include <AkonadiWidgets/CollectionPropertiesDialog>

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
    mPages = QStringList() << QStringLiteral("Akonadi::CollectionGeneralPropertiesPage") << QStringLiteral("Akonadi::CachePolicyPage")
                           << QStringLiteral("PimCommon::CollectionAclPage") << QStringLiteral("Akonadi::CollectionMaintenancePage");
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
    auto sync = new Akonadi::CollectionAttributesSynchronizationJob(col);
    sync->setProperty("collectionId", id);
    connect(sync, &KJob::result, this, &ManageShowCollectionProperties::slotCollectionPropertiesContinued);
    sync->start();
}

void ManageShowCollectionProperties::slotCollectionPropertiesContinued(KJob *job)
{
    if (job) {
        auto sync = qobject_cast<Akonadi::CollectionAttributesSynchronizationJob *>(job);
        Q_ASSERT(sync);
        if (sync->property("collectionId") != mMainWidget->currentAddressBook().id()) {
            return;
        }
    }
    auto fetch = new Akonadi::CollectionFetchJob(mMainWidget->currentAddressBook(), Akonadi::CollectionFetchJob::Base);
    fetch->fetchScope().setIncludeStatistics(true);
    connect(fetch, &KJob::result, this, &ManageShowCollectionProperties::slotCollectionPropertiesFinished);
}

void ManageShowCollectionProperties::slotCollectionPropertiesFinished(KJob *job)
{
    if (!job) {
        return;
    }

    auto fetch = qobject_cast<Akonadi::CollectionFetchJob *>(job);
    Q_ASSERT(fetch);
    if (fetch->collections().isEmpty()) {
        qCWarning(KADDRESSBOOK_LOG) << "no collection";
        return;
    }

    const Akonadi::Collection collection = fetch->collections().constFirst();

    QPointer<Akonadi::CollectionPropertiesDialog> dlg = new Akonadi::CollectionPropertiesDialog(collection, mPages, mMainWidget);
    dlg->setWindowTitle(i18nc("@title:window", "Properties of Address Book Folder %1", collection.name()));

    dlg->show();
    mHashDialogBox.insert(collection.id(), dlg);
}
