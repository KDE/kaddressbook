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

#include "kaddressbookimportexportcontactlist.h"

using namespace KAddressBookImportExport;

KAddressBookImportExportContactList::KAddressBookImportExportContactList()
{
}

bool KAddressBookImportExportContactList::isEmpty() const
{
    return mAddressList.isEmpty() && mContactGroupList.isEmpty();
}

int KAddressBookImportExportContactList::count() const
{
    return mAddressList.count() + mContactGroupList.count();
}

void KAddressBookImportExportContactList::clear()
{
    mAddressList.clear();
    mContactGroupList.clear();
}

void KAddressBookImportExportContactList::append(const KContacts::Addressee &addr)
{
    mAddressList.append(addr);
}

void KAddressBookImportExportContactList::append(const KContacts::ContactGroup &group)
{
    mContactGroupList.append(group);
}

KContacts::ContactGroup::List KAddressBookImportExportContactList::contactGroupList() const
{
    return mContactGroupList;
}

void KAddressBookImportExportContactList::setContactGroupList(const KContacts::ContactGroup::List &value)
{
    mContactGroupList = value;
}

KContacts::Addressee::List KAddressBookImportExportContactList::addressList() const
{
    return mAddressList;
}

void KAddressBookImportExportContactList::setAddressList(const KContacts::Addressee::List &value)
{
    mAddressList = value;
}
