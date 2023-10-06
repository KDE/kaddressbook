/*
  SPDX-FileCopyrightText: 2009 Tobias Koenig <tokoe@kde.org>
  SPDX-FileCopyrightText: 2016-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "contactlist.h"
#include "kaddressbook_importexport_export.h"
#include <Akonadi/Item>

#include <QWidget>

class QCheckBox;
class QItemSelectionModel;
class QLabel;
class QRadioButton;

namespace Akonadi
{
class Collection;
class CollectionComboBox;
}

namespace KAddressBookImportExport
{
/**
 * @short A widget to select a group of contacts.
 *
 * @author Tobias Koenig <tokoe@kde.org>
 */
class KADDRESSBOOK_IMPORTEXPORT_EXPORT ContactSelectionWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * Creates a new contact selection widget.
     *
     * @param selectionModel The model that contains the currently selected contacts.
     * @param parent The parent widget.
     */
    explicit ContactSelectionWidget(QItemSelectionModel *selectionModel, QWidget *parent = nullptr);

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

    void setAddGroupContact(bool addGroupContact);
    [[nodiscard]] Akonadi::Item::List selectedItems() const;

private:
    KADDRESSBOOK_IMPORTEXPORT_NO_EXPORT void initGui();

    [[nodiscard]] KADDRESSBOOK_IMPORTEXPORT_NO_EXPORT ContactList collectAllContacts() const;
    [[nodiscard]] KADDRESSBOOK_IMPORTEXPORT_NO_EXPORT ContactList collectSelectedContacts() const;
    [[nodiscard]] KADDRESSBOOK_IMPORTEXPORT_NO_EXPORT ContactList collectAddressBookContacts() const;

    [[nodiscard]] KADDRESSBOOK_IMPORTEXPORT_NO_EXPORT Akonadi::Item::List collectAllItems() const;
    [[nodiscard]] KADDRESSBOOK_IMPORTEXPORT_NO_EXPORT Akonadi::Item::List collectSelectedItems() const;
    [[nodiscard]] KADDRESSBOOK_IMPORTEXPORT_NO_EXPORT Akonadi::Item::List collectAddressBookItems() const;

    QItemSelectionModel *const mSelectionModel;

    QLabel *mMessageLabel = nullptr;
    QRadioButton *mAllContactsButton = nullptr;
    QRadioButton *mSelectedContactsButton = nullptr;
    QRadioButton *mAddressBookContactsButton = nullptr;
    Akonadi::CollectionComboBox *mAddressBookSelection = nullptr;
    QCheckBox *mAddressBookSelectionRecursive = nullptr;
    bool mAddContactGroup = false;
};
}
