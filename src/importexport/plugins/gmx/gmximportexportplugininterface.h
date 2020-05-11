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

#ifndef GMXIMPORTEXPORTPLUGININTERFACE_H
#define GMXIMPORTEXPORTPLUGININTERFACE_H

#include "kaddressbookimportexportplugininterface.h"

#include <QFile>
#include <KContacts/Addressee>

class GMXImportExportPluginInterface : public KAddressBookImportExport::KAddressBookImportExportPluginInterface
{
    Q_OBJECT
public:
    explicit GMXImportExportPluginInterface(QObject *parent = nullptr);
    ~GMXImportExportPluginInterface() override;

    void createAction(KActionCollection *ac) override;
    void exec() override;
    Q_REQUIRED_RESULT bool canImportFileType(const QUrl &url) override;
private:
    void doExport(QFile *fp, const KContacts::AddresseeList &list) const;
    void slotImportGmx();
    void slotExportGmx();
    void importGMX();
    void exportGMX();
};

#endif // GMXIMPORTEXPORTPLUGININTERFACE_H
