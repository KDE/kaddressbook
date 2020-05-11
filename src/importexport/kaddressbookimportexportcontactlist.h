/*
  This file is part of KAddressBook.

  Copyright (C) 2015-2020 Laurent Montel <montel@kde.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#ifndef KAddressBookImportExportCONTACTLIST_H
#define KAddressBookImportExportCONTACTLIST_H
#include <KContacts/Addressee>
#include <KContacts/ContactGroup>
#include "kaddressbook_export.h"

namespace KAddressBookImportExport {

/** Import/export contact list. */
class KADDRESSBOOK_EXPORT KAddressBookImportExportContactList
{
public:
    KAddressBookImportExportContactList();

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

#endif // KAddressBookImportExportCONTACTLIST_H
