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

#ifndef ADDRESSBOOKLISTVIEWMODEL_H
#define ADDRESSBOOKLISTVIEWMODEL_H

#include <QIdentityProxyModel>

class AddressBookListViewModel : public QIdentityProxyModel
{
    Q_OBJECT
public:
    explicit AddressBookListViewModel(QObject *parent = Q_NULLPTR);
    ~AddressBookListViewModel();
    enum AddressBookRoles {
        AddressEmail = Qt::UserRole + 1,
        AddressName = Qt::UserRole + 2,
        AddressSection = Qt::UserRole + 3,
        AddressIcon = Qt::UserRole + 4
    };

protected:
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
};

#endif // ADDRESSBOOKLISTVIEWMODEL_H
