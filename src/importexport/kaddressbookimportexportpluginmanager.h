/*
   Copyright (C) 2016-2020 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef KADDRESSBOOKIMPORTEXPORTPLUGINMANAGER_H
#define KADDRESSBOOKIMPORTEXPORTPLUGINMANAGER_H

#include <QObject>
#include <PimCommon/PluginUtil>
#include "kaddressbook_export.h"

namespace KAddressBookImportExport {
class KAddressBookImportExportPlugin;
class KAddressBookImportExportPrivate;

/** Contact import/export plugin manager. */
class KADDRESSBOOK_EXPORT KAddressBookImportExportPluginManager : public QObject
{
    Q_OBJECT
public:
    explicit KAddressBookImportExportPluginManager(QObject *parent = nullptr);
    ~KAddressBookImportExportPluginManager();
    static KAddressBookImportExportPluginManager *self();

    Q_REQUIRED_RESULT QVector<KAddressBookImportExportPlugin *> pluginsList() const;

    Q_REQUIRED_RESULT QVector<PimCommon::PluginUtilData> pluginsDataList() const;

    Q_REQUIRED_RESULT QString configGroupName() const;
    Q_REQUIRED_RESULT QString configPrefixSettingKey() const;
    KAddressBookImportExportPlugin *pluginFromIdentifier(const QString &id);
private:
    KAddressBookImportExportPrivate *const d;
};
}

#endif // KADDRESSBOOKIMPORTEXPORTPLUGINMANAGER_H
