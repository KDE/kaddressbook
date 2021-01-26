/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kaddressbookmigrateapplication.h"

#include <Kdelibs4ConfigMigrator>

KAddressBookMigrateApplication::KAddressBookMigrateApplication()
{
    initializeMigrator();
}

void KAddressBookMigrateApplication::migrate()
{
    // Migrate to xdg.
    Kdelibs4ConfigMigrator migrate(QStringLiteral("kaddressbook"));
    migrate.setConfigFiles(QStringList() << QStringLiteral("kaddressbookrc"));
    migrate.setUiFiles(QStringList() << QStringLiteral("kaddressbookui.rc"));
    migrate.migrate();

    // Migrate folders and files.
    if (mMigrator.checkIfNecessary()) {
        mMigrator.start();
    }
}

void KAddressBookMigrateApplication::initializeMigrator()
{
    const int currentVersion = 2;
    mMigrator.setApplicationName(QStringLiteral("kaddressbook"));
    mMigrator.setConfigFileName(QStringLiteral("kaddressbookrc"));

    // To migrate we need a version > currentVersion
    const int initialVersion = currentVersion + 1;

    // printing
    PimCommon::MigrateFileInfo migrateInfoPrinting;
    migrateInfoPrinting.setFolder(true);
    migrateInfoPrinting.setType(QStringLiteral("data"));
    migrateInfoPrinting.setPath(QStringLiteral("kaddressbook/printing"));
    migrateInfoPrinting.setVersion(initialVersion);
    mMigrator.insertMigrateInfo(migrateInfoPrinting);

    // viewertemplates
    PimCommon::MigrateFileInfo migrateInfoViewerTemplates;
    migrateInfoViewerTemplates.setFolder(true);
    migrateInfoViewerTemplates.setType(QStringLiteral("data"));
    migrateInfoViewerTemplates.setPath(QStringLiteral("kaddressbook/viewertemplates"));
    migrateInfoViewerTemplates.setVersion(initialVersion);
    mMigrator.insertMigrateInfo(migrateInfoViewerTemplates);

    // viewertemplates
    PimCommon::MigrateFileInfo migrateInfoCsvTemplates;
    migrateInfoCsvTemplates.setFolder(true);
    migrateInfoCsvTemplates.setType(QStringLiteral("data"));
    migrateInfoCsvTemplates.setPath(QStringLiteral("kaddressbook/csv-templates"));
    migrateInfoCsvTemplates.setVersion(initialVersion);
    mMigrator.insertMigrateInfo(migrateInfoCsvTemplates);

    mMigrator.setCurrentConfigVersion(currentVersion);

    // TODO add folder to migrate
}
