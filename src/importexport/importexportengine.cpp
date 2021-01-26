/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importexportengine.h"

#include <AkonadiCore/Item>
#include <AkonadiCore/ItemCreateJob>
#include <AkonadiWidgets/CollectionDialog>
#include <KLocalizedString>
#include <QPointer>
#include <QProgressDialog>

using namespace KAddressBookImportExport;

ImportExportEngine::ImportExportEngine(QObject *parent)
    : QObject(parent)
{
}

ImportExportEngine::~ImportExportEngine()
{
}

void ImportExportEngine::setContactList(const ContactList &contacts)
{
    mContactsList = contacts;
}

void ImportExportEngine::setParentWidget(QWidget *parent)
{
    mParentWidget = parent;
}

QWidget *ImportExportEngine::parentWidget() const
{
    return mParentWidget;
}

void ImportExportEngine::setDefaultAddressBook(const Akonadi::Collection &defaultAddressBook)
{
    mDefaultAddressBook = defaultAddressBook;
}

void ImportExportEngine::importContacts()
{
    if (mContactsList.isEmpty()) { // nothing to import
        Q_EMIT finished();
        deleteLater();
        return;
    }

    const QStringList mimeTypes(KContacts::Addressee::mimeType());

    QPointer<Akonadi::CollectionDialog> dlg = new Akonadi::CollectionDialog(mParentWidget);
    dlg->setMimeTypeFilter(mimeTypes);
    dlg->setAccessRightsFilter(Akonadi::Collection::CanCreateItem);
    dlg->setWindowTitle(i18nc("@title:window", "Select Address Book"));
    dlg->setDescription(i18n("Select the address book the imported contact(s) shall be saved in:"));
    dlg->setDefaultCollection(mDefaultAddressBook);

    if (!dlg->exec()) {
        delete dlg;
        deleteLater();
        return;
    }

    const Akonadi::Collection collection = dlg->selectedCollection();
    delete dlg;

    mNumberElementToImport = mContactsList.count();
    if (!mImportProgressDialog) {
        mImportProgressDialog = new QProgressDialog(mParentWidget);
        mImportProgressDialog->setWindowTitle(i18nc("@title:window", "Import Contacts"));
        mImportProgressDialog->setLabelText(i18np("Importing one contact to %2", "Importing %1 contacts to %2", mNumberElementToImport, collection.name()));
        mImportProgressDialog->setCancelButton(nullptr);
        mImportProgressDialog->setAutoClose(true);
        mImportProgressDialog->setRange(1, mNumberElementToImport);
    }

    mImportProgressDialog->show();
    mImportDone = 0;

    for (int i = 0; i < mContactsList.addressList().count(); ++i) {
        Akonadi::Item item;
        item.setPayload<KContacts::Addressee>(mContactsList.addressList().at(i));
        item.setMimeType(KContacts::Addressee::mimeType());

        auto job = new Akonadi::ItemCreateJob(item, collection);
        connect(job, &Akonadi::ItemCreateJob::result, this, &ImportExportEngine::slotImportJobDone);
    }
    const int numberGroup(mContactsList.contactGroupList().count());
    for (int i = 0; i < numberGroup; ++i) {
        Akonadi::Item groupItem(KContacts::ContactGroup::mimeType());
        groupItem.setPayload<KContacts::ContactGroup>(mContactsList.contactGroupList().at(i));

        Akonadi::Job *createJob = new Akonadi::ItemCreateJob(groupItem, collection);
        connect(createJob, &KJob::result, this, &ImportExportEngine::slotImportJobDone);
    }
}

void ImportExportEngine::slotImportJobDone(KJob *)
{
    mImportDone++;
    if (mImportProgressDialog) {
        mImportProgressDialog->setValue(mImportProgressDialog->value() + 1);

        // cleanup on last step
        if (mImportProgressDialog->value() == mImportProgressDialog->maximum()) {
            mImportProgressDialog->deleteLater();
            mImportProgressDialog = nullptr;
        }
    }
    if (mImportDone >= mNumberElementToImport) {
        Q_EMIT finished();
        deleteLater();
    }
}
