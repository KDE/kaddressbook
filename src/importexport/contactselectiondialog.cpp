/*
  SPDX-FileCopyrightText: 2009 Tobias Koenig <tokoe@kde.org>
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "contactselectiondialog.h"
#include "contactselectionwidget.h"

#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

using namespace KAddressBookImportExport;

ContactSelectionDialog::ContactSelectionDialog(QItemSelectionModel *selectionModel, bool allowToSelectTypeToExport, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Select Contacts"));
    auto mainLayout = new QVBoxLayout(this);

    mSelectionWidget = new ContactSelectionWidget(selectionModel, this);
    if (allowToSelectTypeToExport) {
        mainLayout->addWidget(mSelectionWidget);
        mVCardExport = new ExportSelectionWidget(this);
        mainLayout->addWidget(mVCardExport);
    } else {
        mainLayout->addWidget(mSelectionWidget);
    }

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &ContactSelectionDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ContactSelectionDialog::reject);
    mainLayout->addWidget(buttonBox);
}

void ContactSelectionDialog::setMessageText(const QString &message)
{
    mSelectionWidget->setMessageText(message);
}

void ContactSelectionDialog::setDefaultAddressBook(const Akonadi::Collection &addressBook)
{
    mSelectionWidget->setDefaultAddressBook(addressBook);
}

Akonadi::Item::List ContactSelectionDialog::selectedItems() const
{
    return mSelectionWidget->selectedItems();
}

ContactList ContactSelectionDialog::selectedContacts() const
{
    return mSelectionWidget->selectedContacts();
}

ExportSelectionWidget::ExportFields ContactSelectionDialog::exportType() const
{
    if (mVCardExport) {
        return mVCardExport->exportType();
    } else {
        return ExportSelectionWidget::None;
    }
}

void ContactSelectionDialog::setAddGroupContact(bool addGroupContact)
{
    mSelectionWidget->setAddGroupContact(addGroupContact);
}
