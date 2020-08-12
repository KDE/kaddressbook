/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2009 Tobias Koenig <tokoe@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef CONTACTSORTER_H
#define CONTACTSORTER_H

#include "kaddressbookimportexportcontactfields.h"

class ContactSorter
{
public:
    explicit ContactSorter(KAddressBookImportExport::KAddressBookImportExportContactFields::Field field, Qt::SortOrder sortOrder = Qt::AscendingOrder);

    void sort(KContacts::Addressee::List &contacts) const;

private:
    const KAddressBookImportExport::KAddressBookImportExportContactFields::Field mSortField;
    const Qt::SortOrder mSortOrder;
};

#endif
