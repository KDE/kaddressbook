/*
   Copyright (C) 2016-2020 Laurent Montel <montel@kde.org>

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

#include "importexportengine.h"

#include <AkonadiWidgets/CollectionDialog>
#include <AkonadiCore/ItemCreateJob>
#include <KLocalizedString>
#include <QProgressDialog>
#include <QPointer>
#include <AkonadiCore/Item>

ImportExportEngine::ImportExportEngine(QObject *parent)
    : QObject(parent)
{
}

ImportExportEngine::~ImportExportEngine()
{
}

void ImportExportEngine::setContactList(const KAddressBookImportExport::KAddressBookImportExportContactList &contacts)
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
    if (mContactsList.isEmpty()) {   // nothing to import
        Q_EMIT finished();
        deleteLater();
        return;
    }

    const QStringList mimeTypes(KContacts::Addressee::mimeType());

    QPointer<Akonadi::CollectionDialog> dlg = new Akonadi::CollectionDialog(mParentWidget);
    dlg->setMimeTypeFilter(mimeTypes);
    dlg->setAccessRightsFilter(Akonadi::Collection::CanCreateItem);
    dlg->setWindowTitle(i18nc("@title:window", "Select Address Book"));
    dlg->setDescription(
        i18n("Select the address book the imported contact(s) shall be saved in:"));
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
        mImportProgressDialog->setLabelText(
            i18np("Importing one contact to %2", "Importing %1 contacts to %2",
                  mNumberElementToImport, collection.name()));
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

        Akonadi::ItemCreateJob *job = new Akonadi::ItemCreateJob(item, collection);
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
