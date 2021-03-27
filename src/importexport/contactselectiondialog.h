/*
  SPDX-FileCopyrightText: 2009 Tobias Koenig <tokoe@kde.org>
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "contactlist.h"
#include "exportselectionwidget.h"
#include "kaddressbook_importexport_export.h"

#include <AkonadiCore/Item>
#include <KContacts/Addressee>
#include <QDialog>

class QItemSelectionModel;
namespace Akonadi
{
class Collection;
}

namespace KAddressBookImportExport
{
class ContactSelectionWidget;

/**
 * @short A dialog to select a group of contacts.
 *
 * @author Tobias Koenig <tokoe@kde.org>
 */
class KADDRESSBOOK_IMPORTEXPORT_EXPORT ContactSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * Creates a new contact selection dialog.
     *
     * @param selectionModel The model that contains the currently selected contacts.
     * @param parent The parent widget.
     */
    explicit ContactSelectionDialog(QItemSelectionModel *selectionModel, bool allowToSelectTypeToExport, QWidget *parent = nullptr);

    /**
     * Sets the @p message text.
     */
    void setMessageText(const QString &message);

    /**
     * Sets the default addressbook.
     */
    void setDefaultAddressBook(const Akonadi::Collection &addressBook);

    /**
     * Returns the list of selected contacts.
     */
    Q_REQUIRED_RESULT ContactList selectedContacts() const;

    Q_REQUIRED_RESULT ExportSelectionWidget::ExportFields exportType() const;
    void setAddGroupContact(bool addGroupContact);

    Q_REQUIRED_RESULT Akonadi::Item::List selectedItems() const;

private:
    ContactSelectionWidget *mSelectionWidget = nullptr;
    ExportSelectionWidget *mVCardExport = nullptr;
};
}
