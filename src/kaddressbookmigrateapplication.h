/*
   SPDX-FileCopyrightText: 2015-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "kaddressbook_export.h"
#include <QObject>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <PimCommon/MigrateApplicationFiles>

class KADDRESSBOOK_EXPORT KAddressBookMigrateApplication
{
public:
    KAddressBookMigrateApplication();
    void migrate();

private:
    void initializeMigrator();
    PimCommon::MigrateApplicationFiles mMigrator;
};
#endif
