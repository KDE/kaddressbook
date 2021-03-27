/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

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

    Q_REQUIRED_RESULT bool isEmpty() const;
    Q_REQUIRED_RESULT int count() const;
    void clear();

    void append(const KContacts::Addressee &addr);
    void append(const KContacts::ContactGroup &group);

    Q_REQUIRED_RESULT KContacts::Addressee::List addressList() const;
    void setAddressList(const KContacts::Addressee::List &value);
    Q_REQUIRED_RESULT KContacts::ContactGroup::List contactGroupList() const;
    void setContactGroupList(const KContacts::ContactGroup::List &value);

private:
    KContacts::Addressee::List mAddressList;
    KContacts::ContactGroup::List mContactGroupList;
};
}

