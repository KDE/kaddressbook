/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2009 Tobias Koenig <tokoe@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
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
