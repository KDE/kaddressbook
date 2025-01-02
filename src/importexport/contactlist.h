/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "kaddressbook_importexport_export.h"
#include <KContacts/Addressee>
#include <KContacts/ContactGroup>

namespace KAddressBookImportExport
{
/** Import/export contact list. */
class KADDRESSBOOK_IMPORTEXPORT_EXPORT ContactList
{
public:
    explicit ContactList();

    [[nodiscard]] bool isEmpty() const;
    [[nodiscard]] int count() const;
    void clear();

    void append(const KContacts::Addressee &addr);
    void append(const KContacts::ContactGroup &group);

    [[nodiscard]] KContacts::Addressee::List addressList() const;
    void setAddressList(const KContacts::Addressee::List &value);
    [[nodiscard]] KContacts::ContactGroup::List contactGroupList() const;
    void setContactGroupList(const KContacts::ContactGroup::List &value);

private:
    KContacts::Addressee::List mAddressList;
    KContacts::ContactGroup::List mContactGroupList;
};
}
