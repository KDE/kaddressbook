/*
  SPDX-FileCopyrightText: 2009 Tobias Koenig <tokoe@kde.org>
  SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "contactlist.h"
#include "exportselectionwidget.h"
#include "kaddressbook_importexport_export.h"

#include <Akonadi/Item>
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
    [[nodiscard]] ContactList selectedContacts() const;

    [[nodiscard]] ExportSelectionWidget::ExportFields exportType() const;
    void setAddGroupContact(bool addGroupContact);

    [[nodiscard]] Akonadi::Item::List selectedItems() const;

private:
    ContactSelectionWidget *const mSelectionWidget;
    ExportSelectionWidget *mVCardExport = nullptr;
};
}
