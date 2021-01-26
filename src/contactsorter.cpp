/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2009 Tobias Koenig <tokoe@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "contactsorter.h"

#include <KContacts/Addressee>

using namespace KAddressBookImportExport;

class ContactSortHelper
{
public:
    ContactSortHelper(ContactFields::Field field, Qt::SortOrder sortOrder)
        : mSortField(field)
        , mSortOrder(sortOrder)
    {
    }

    inline bool operator()(const KContacts::Addressee &contact, const KContacts::Addressee &otherContact) const
    {
        int result = QString::localeAwareCompare(ContactFields::value(mSortField, contact), ContactFields::value(mSortField, otherContact));

        if (result == 0) {
            int givenNameResult = QString::localeAwareCompare(ContactFields::value(ContactFields::GivenName, contact),
                                                              ContactFields::value(ContactFields::GivenName, otherContact));

            if (givenNameResult == 0) {
                int familyNameResult = QString::localeAwareCompare(ContactFields::value(ContactFields::FamilyName, contact),
                                                                   ContactFields::value(ContactFields::FamilyName, otherContact));

                if (familyNameResult == 0) {
                    result = QString::localeAwareCompare(ContactFields::value(ContactFields::FormattedName, contact),
                                                         ContactFields::value(ContactFields::FormattedName, otherContact));
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
    const ContactFields::Field mSortField;
    const Qt::SortOrder mSortOrder;
};

ContactSorter::ContactSorter(ContactFields::Field field, Qt::SortOrder sortOrder)
    : mSortField(field)
    , mSortOrder(sortOrder)
{
}

void ContactSorter::sort(KContacts::Addressee::List &contacts) const
{
    std::stable_sort(contacts.begin(), contacts.end(), ContactSortHelper(mSortField, mSortOrder));
}
