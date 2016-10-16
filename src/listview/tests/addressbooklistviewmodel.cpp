/*
    Copyright (C) 2016 Laurent Montel <montel@kde.org>

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

#include "addressbooklistviewmodel.h"

AddressBookListViewModel::AddressBookListViewModel(QObject *parent)
    : QIdentityProxyModel(parent)
{

}

AddressBookListViewModel::~AddressBookListViewModel()
{

}

QHash<int, QByteArray> AddressBookListViewModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[AddressEmail] = "addressemail";
    roles[AddressName] = "addressname";
    roles[AddressSection] = "addresssection";
    roles[AddressIcon] = "addressicon";
    return roles;
}

QVariant AddressBookListViewModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0) {
        return QVariant();
    }
    //TODO
    if (role == AddressEmail) {
        return {};
    } else if (role == AddressName) {
        return {};
    } else if (role == AddressSection) {
        return {};
    } else if (role == AddressIcon) {
        return {};
    }
    //TODO
#if 0
    if (index.row() < 0 || index.row() >= mListAddressBook.count()) {
        return QVariant();
    }
    const AddressBook &add = mListAddressBook[index.row()];
    if (role == AddressEmail) {
        return add.email();
    } else if (role == AddressName) {
        return add.name();
    } else if (role == AddressSection) {
        return add.section();
    } else if (role == AddressIcon) {
        return add.iconUrl();
    }
#endif
    return QVariant();
}
