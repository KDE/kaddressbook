/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2009 Tobias Koenig <tokoe@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "importexport/contactfields.h"

class ContactSorter
{
public:
    explicit ContactSorter(KAddressBookImportExport::ContactFields::Field field, Qt::SortOrder sortOrder = Qt::AscendingOrder);

    void sort(KContacts::Addressee::List &contacts) const;

private:
    const KAddressBookImportExport::ContactFields::Field mSortField;
    const Qt::SortOrder mSortOrder;
};

