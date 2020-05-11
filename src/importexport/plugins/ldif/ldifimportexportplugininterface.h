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

#ifndef LDIFIMPORTEXPORTPLUGININTERFACE_H
#define LDIFIMPORTEXPORTPLUGININTERFACE_H

#include "kaddressbookimportexportplugininterface.h"

class LDifImportExportPluginInterface : public KAddressBookImportExport::KAddressBookImportExportPluginInterface
{
    Q_OBJECT
public:
    explicit LDifImportExportPluginInterface(QObject *parent = nullptr);
    ~LDifImportExportPluginInterface() override;

    void createAction(KActionCollection *ac) override;
    void exec() override;
    Q_REQUIRED_RESULT bool canImportFileType(const QUrl &url) override;
    void importFile(const QUrl &url) override;
private:
    void slotImportLdif();
    void slotExportLdif();
    void importLdif();
    void exportLdif();
    void importLdifFile(const QString &fileName);
};

#endif // LDIFIMPORTEXPORTPLUGININTERFACE_H
