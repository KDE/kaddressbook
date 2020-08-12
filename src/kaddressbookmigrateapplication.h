/*
   SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KADDRESSBOOKMIGRATEAPPLICATION_H
#define KADDRESSBOOKMIGRATEAPPLICATION_H

#include <PimCommon/MigrateApplicationFiles>
#include "kaddressbook_export.h"

class KADDRESSBOOK_EXPORT KAddressBookMigrateApplication
{
public:
    KAddressBookMigrateApplication();
    void migrate();
private:
    void initializeMigrator();
    PimCommon::MigrateApplicationFiles mMigrator;
};

#endif // KADDRESSBOOKMIGRATEAPPLICATION_H
