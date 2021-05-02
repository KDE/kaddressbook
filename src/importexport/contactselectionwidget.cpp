/*
  SPDX-FileCopyrightText: 2009 Tobias Koenig <tokoe@kde.org>
  SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "contactselectionwidget.h"
#include <QDebug>

#include <AkonadiCore/EntityTreeModel>
#include <AkonadiCore/ItemFetchJob>
#include <AkonadiCore/ItemFetchScope>
#include <AkonadiCore/RecursiveItemFetchJob>
#include <AkonadiWidgets/CollectionComboBox>

#include <KLocalizedString>

#include <QApplication>
#include <QButtonGroup>
#include <QCheckBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QItemSelectionModel>
#include <QLabel>
#include <QProgressDialog>
#include <QRadioButton>
#include <QVBoxLayout>

using namespace KAddressBookImportExport;

ContactSelectionWidget::ContactSelectionWidget(QItemSelectionModel *selectionModel, QWidget *parent)
    : QWidget(parent)
    , mSelectionModel(selectionModel)
{
    initGui();

    mSelectedContactsButton->setEnabled(mSelectionModel->hasSelection());
    mAddressBookSelection->setEnabled(false);
    mAddressBookSelectionRecursive->setEnabled(false);

    connect(mAddressBookContactsButton, &QRadioButton::toggled, mAddressBookSelection, &Akonadi::CollectionComboBox::setEnabled);
    connect(mAddressBookContactsButton, &QRadioButton::toggled, mAddressBookSelectionRecursive, &QCheckBox::setEnabled);

    // apply default configuration
    if (mSelectionModel->hasSelection()) {
        mSelectedContactsButton->setChecked(true);
    } else {
        mAllContactsButton->setChecked(true);
    }
}

void ContactSelectionWidget::setMessageText(const QString &message)
{
    if (!message.isEmpty()) {
        mMessageLabel->setText(message);
        mMessageLabel->show();
    }
}

void ContactSelectionWidget::setDefaultAddressBook(const Akonadi::Collection &addressBook)
{
    mAddressBookSelection->setDefaultCollection(addressBook);
}

Akonadi::Item::List ContactSelectionWidget::selectedItems() const
{
    if (mAllContactsButton->isChecked()) {
        return collectAllItems();
    } else if (mSelectedContactsButton->isChecked()) {
        return collectSelectedItems();
    } else if (mAddressBookContactsButton->isChecked()) {
        return collectAddressBookItems();
    } else {
        qWarning() << " It's a bug here.";
    }

    return Akonadi::Item::List();
}

ContactList ContactSelectionWidget::selectedContacts() const
{
    if (mAllContactsButton->isChecked()) {
        return collectAllContacts();
    } else if (mSelectedContactsButton->isChecked()) {
        return collectSelectedContacts();
    } else if (mAddressBookContactsButton->isChecked()) {
        return collectAddressBookContacts();
    }

    return ContactList();
}

void ContactSelectionWidget::setAddGroupContact(bool addGroupContact)
{
    mAddContactGroup = addGroupContact;
}

void ContactSelectionWidget::initGui()
{
    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    mMessageLabel = new QLabel;
    layout->addWidget(mMessageLabel);
    mMessageLabel->hide();

    auto group = new QButtonGroup(this);

    auto groupBox = new QGroupBox;

    auto boxLayout = new QGridLayout;
    groupBox->setLayout(boxLayout);

    mAllContactsButton = new QRadioButton(i18nc("@option:radio", "All contacts"));
    mAllContactsButton->setToolTip(i18nc("@info:tooltip", "All contacts from all your address books"));
    mAllContactsButton->setWhatsThis(i18nc("@info:whatsthis",
                                           "Choose this option you want to select all your contacts from "
                                           "all your address books."));

    mSelectedContactsButton = new QRadioButton(i18nc("@option:radio", "Selected contacts"));
    mSelectedContactsButton->setToolTip(i18nc("@info:tooltip", "Only the contacts currently selected"));
    mSelectedContactsButton->setWhatsThis(i18nc("@info:whatsthis",
                                                "Choose this option if you want only the contacts you have already "
                                                "selected in the graphical interface."));

    mAddressBookContactsButton = new QRadioButton(i18nc("@option:radio", "All contacts from:"));
    mAddressBookContactsButton->setToolTip(i18nc("@info:tooltip", "All contacts from a chosen address book"));
    mAddressBookContactsButton->setWhatsThis(i18nc("@info:whatsthis",
                                                   "Choose this option if you want to select all the contacts from only one "
                                                   "of your address books.  Once this option is clicked you will be provided "
                                                   "a drop down box listing all those address books and permitted to select "
                                                   "the one you want."));

    mAddressBookSelection = new Akonadi::CollectionComboBox;
    mAddressBookSelection->setMimeTypeFilter(QStringList() << KContacts::Addressee::mimeType());
    mAddressBookSelection->setAccessRightsFilter(Akonadi::Collection::ReadOnly);
    mAddressBookSelection->setExcludeVirtualCollections(true);
    mAddressBookSelectionRecursive = new QCheckBox(i18nc("@option:check", "Include Subfolders"));
    mAddressBookSelectionRecursive->setToolTip(i18nc("@info:tooltip", "Select all subfolders including the top-level folder"));
    mAddressBookSelectionRecursive->setWhatsThis(i18nc("@info:whatsthis",
                                                       "Check this box if you want to select all contacts from this folder, "
                                                       "including all subfolders.  If you only want the contacts from the "
                                                       "top-level folder then leave this box unchecked."));

    group->addButton(mAllContactsButton);
    group->addButton(mSelectedContactsButton);
    group->addButton(mAddressBookContactsButton);

    boxLayout->addWidget(mAllContactsButton, 0, 0, 1, 2);
    boxLayout->addWidget(mSelectedContactsButton, 1, 0, 1, 2);
    boxLayout->addWidget(mAddressBookContactsButton, 2, 0, Qt::AlignTop);

    auto addressBookLayout = new QVBoxLayout;
    addressBookLayout->setContentsMargins(0, 0, 0, 0);
    addressBookLayout->addWidget(mAddressBookSelection);
    addressBookLayout->addWidget(mAddressBookSelectionRecursive);

    boxLayout->addLayout(addressBookLayout, 2, 1);

    layout->addWidget(groupBox);
    layout->addStretch(1);
}

Akonadi::Item::List ContactSelectionWidget::collectAllItems() const
{
    auto job = new Akonadi::RecursiveItemFetchJob(Akonadi::Collection::root(), QStringList() << KContacts::Addressee::mimeType());
    job->fetchScope().fetchFullPayload();
    QProgressDialog progressDialog(nullptr);
    progressDialog.setWindowTitle(i18nc("@title:window", "Collect Contacts"));
    progressDialog.setAutoClose(true);
    progressDialog.setMinimumDuration(1000);
    progressDialog.setLabelText(i18n("Fetch Contacts"));
    progressDialog.show();

    qApp->processEvents();

    if (!job->exec()) {
        return Akonadi::Item::List();
    }

    return job->items();
}

ContactList ContactSelectionWidget::collectAllContacts() const
{
    ContactList contacts;
    auto job = new Akonadi::RecursiveItemFetchJob(Akonadi::Collection::root(), QStringList() << KContacts::Addressee::mimeType());
    job->fetchScope().fetchFullPayload();

    if (!job->exec()) {
        return contacts;
    }
    const Akonadi::Item::List lstItems = job->items();
    for (const Akonadi::Item &item : lstItems) {
        if (item.isValid()) {
            if (item.hasPayload<KContacts::Addressee>()) {
                contacts.append(item.payload<KContacts::Addressee>());
            }
        }
    }

    return contacts;
}

Akonadi::Item::List ContactSelectionWidget::collectSelectedItems() const
{
    Akonadi::Item::List items;

    const QModelIndexList indexes = mSelectionModel->selectedRows(0);
    for (int i = 0, total = indexes.count(); i < total; ++i) {
        const QModelIndex index = indexes.at(i);
        if (index.isValid()) {
            const auto item = index.data(Akonadi::EntityTreeModel::ItemRole).value<Akonadi::Item>();
            if (item.isValid()) {
                items.append(item);
            }
        }
    }

    return items;
}

ContactList ContactSelectionWidget::collectSelectedContacts() const
{
    ContactList contacts;

    const QModelIndexList indexes = mSelectionModel->selectedRows(0);
    for (int i = 0, total = indexes.count(); i < total; ++i) {
        const QModelIndex index = indexes.at(i);
        if (index.isValid()) {
            const auto item = index.data(Akonadi::EntityTreeModel::ItemRole).value<Akonadi::Item>();
            if (item.isValid()) {
                if (item.hasPayload<KContacts::Addressee>()) {
                    contacts.append(item.payload<KContacts::Addressee>());
                }
            }
        }
    }

    return contacts;
}

Akonadi::Item::List ContactSelectionWidget::collectAddressBookItems() const
{
    Akonadi::Item::List items;

    const Akonadi::Collection collection = mAddressBookSelection->currentCollection();
    if (!collection.isValid()) {
        return items;
    }

    if (mAddressBookSelectionRecursive->isChecked()) {
        auto job = new Akonadi::RecursiveItemFetchJob(collection, QStringList() << KContacts::Addressee::mimeType());
        job->fetchScope().fetchFullPayload();

        if (!job->exec()) {
            return items;
        }

        items = job->items();
    } else {
        auto job = new Akonadi::ItemFetchJob(collection);
        job->fetchScope().fetchFullPayload();

        if (!job->exec()) {
            return items;
        }
        items = job->items();
    }

    return items;
}

ContactList ContactSelectionWidget::collectAddressBookContacts() const
{
    ContactList contacts;

    const Akonadi::Collection collection = mAddressBookSelection->currentCollection();
    if (!collection.isValid()) {
        return contacts;
    }

    if (mAddressBookSelectionRecursive->isChecked()) {
        auto job = new Akonadi::RecursiveItemFetchJob(collection, QStringList() << KContacts::Addressee::mimeType());
        job->fetchScope().fetchFullPayload();

        if (!job->exec()) {
            return contacts;
        }

        const Akonadi::Item::List lstItems = job->items();
        for (const Akonadi::Item &item : lstItems) {
            if (item.hasPayload<KContacts::Addressee>()) {
                contacts.append(item.payload<KContacts::Addressee>());
            }
        }
    } else {
        auto job = new Akonadi::ItemFetchJob(collection);
        job->fetchScope().fetchFullPayload();

        if (!job->exec()) {
            return contacts;
        }
        const Akonadi::Item::List lstItems = job->items();
        for (const Akonadi::Item &item : lstItems) {
            if (item.hasPayload<KContacts::Addressee>()) {
                contacts.append(item.payload<KContacts::Addressee>());
            }
        }
    }

    return contacts;
}
