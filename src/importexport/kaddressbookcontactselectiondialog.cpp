/*
  Copyright (c) 2009 Tobias Koenig <tokoe@kde.org>
  Copyright (C) 2015-2020 Laurent Montel <montel@kde.org>

  This library is free software; you can redistribute it and/or modify it
  under the terms of the GNU Library General Public License as published by
  the Free Software Foundation; either version 2 of the License, or (at your
  option) any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
  License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to the
  Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
  02110-1301, USA.
*/

#include "kaddressbookcontactselectiondialog.h"
#include "kaddressbookcontactselectionwidget.h"

#include <QVBoxLayout>
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QPushButton>
using namespace KAddressBookImportExport;

KAddressBookContactSelectionDialog::KAddressBookContactSelectionDialog(QItemSelectionModel *selectionModel, bool allowToSelectTypeToExport, QWidget *parent)
    : QDialog(parent)
    , mVCardExport(nullptr)
{
    setWindowTitle(i18nc("@title:window", "Select Contacts"));
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    mSelectionWidget = new KAddressBookContactSelectionWidget(selectionModel, this);
    if (allowToSelectTypeToExport) {
        mainLayout->addWidget(mSelectionWidget);
        mVCardExport = new KAddressBookExportSelectionWidget(this);
        mainLayout->addWidget(mVCardExport);
    } else {
        mainLayout->addWidget(mSelectionWidget);
    }

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &KAddressBookContactSelectionDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &KAddressBookContactSelectionDialog::reject);
    mainLayout->addWidget(buttonBox);
}

void KAddressBookContactSelectionDialog::setMessageText(const QString &message)
{
    mSelectionWidget->setMessageText(message);
}

void KAddressBookContactSelectionDialog::setDefaultAddressBook(const Akonadi::Collection &addressBook)
{
    mSelectionWidget->setDefaultAddressBook(addressBook);
}

Akonadi::Item::List KAddressBookContactSelectionDialog::selectedItems() const
{
    return mSelectionWidget->selectedItems();
}

KAddressBookImportExport::KAddressBookImportExportContactList KAddressBookContactSelectionDialog::selectedContacts() const
{
    return mSelectionWidget->selectedContacts();
}

KAddressBookExportSelectionWidget::ExportFields KAddressBookContactSelectionDialog::exportType() const
{
    if (mVCardExport) {
        return mVCardExport->exportType();
    } else {
        return KAddressBookExportSelectionWidget::None;
    }
}

void KAddressBookContactSelectionDialog::setAddGroupContact(bool addGroupContact)
{
    mSelectionWidget->setAddGroupContact(addGroupContact);
}
