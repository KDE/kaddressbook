/*
  This file is part of KAddressBook.

  Copyright (c) 2009 Tobias Koenig <tokoe@kde.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#include "contactsorter.h"

#include <KContacts/Addressee>

class ContactSortHelper
{
public:
    ContactSortHelper(KAddressBookImportExport::KAddressBookImportExportContactFields::Field field, Qt::SortOrder sortOrder)
        : mSortField(field)
        , mSortOrder(sortOrder)
    {
    }

    inline bool operator()(const KContacts::Addressee &contact, const KContacts::Addressee &otherContact) const
    {
        int result
            = QString::localeAwareCompare(
                  KAddressBookImportExport::KAddressBookImportExportContactFields::value(mSortField, contact),
                  KAddressBookImportExport::KAddressBookImportExportContactFields::value(mSortField, otherContact));

        if (result == 0) {
            int givenNameResult
                = QString::localeAwareCompare(
                      KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::GivenName, contact),
                      KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::GivenName, otherContact));

            if (givenNameResult == 0) {
                int familyNameResult
                    = QString::localeAwareCompare(
                          KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::FamilyName, contact),
                          KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::FamilyName, otherContact));

                if (familyNameResult == 0) {
                    result
                        = QString::localeAwareCompare(
                              KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::FormattedName, contact),
                              KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::FormattedName, otherContact));
                } else {
                    result = familyNameResult;
                }
            } else {
                result = givenNameResult;
            }
        }

        bool lesser = result < 0;

        if (mSortOrder == Qt::DescendingOrder) {
            lesser = !lesser;
        }

        return lesser;
    }

private:
    const KAddressBookImportExport::KAddressBookImportExportContactFields::Field mSortField;
    const Qt::SortOrder mSortOrder;
};

ContactSorter::ContactSorter(KAddressBookImportExport::KAddressBookImportExportContactFields::Field field, Qt::SortOrder sortOrder)
    : mSortField(field)
    , mSortOrder(sortOrder)
{
}

void ContactSorter::sort(KContacts::Addressee::List &contacts) const
{
    std::stable_sort(contacts.begin(), contacts.end(), ContactSortHelper(mSortField, mSortOrder));
}
