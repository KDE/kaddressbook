/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "contactlist.h"

using namespace KAddressBookImportExport;

ContactList::ContactList() = default;

bool ContactList::isEmpty() const
{
    return mAddressList.isEmpty() && mContactGroupList.isEmpty();
}

int ContactList::count() const
{
    return mAddressList.count() + mContactGroupList.count();
}

void ContactList::clear()
{
    mAddressList.clear();
    mContactGroupList.clear();
}

void ContactList::append(const KContacts::Addressee &addr)
{
    mAddressList.append(addr);
}

void ContactList::append(const KContacts::ContactGroup &group)
{
    mContactGroupList.append(group);
}

KContacts::ContactGroup::List ContactList::contactGroupList() const
{
    return mContactGroupList;
}

void ContactList::setContactGroupList(const KContacts::ContactGroup::List &value)
{
    mContactGroupList = value;
}

KContacts::Addressee::List ContactList::addressList() const
{
    return mAddressList;
}

void ContactList::setAddressList(const KContacts::Addressee::List &value)
{
    mAddressList = value;
}
