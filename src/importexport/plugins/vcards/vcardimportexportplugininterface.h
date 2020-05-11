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

#ifndef VCARDIMPORTEXPORTPLUGININTERFACE_H
#define VCARDIMPORTEXPORTPLUGININTERFACE_H

#include "kaddressbookimportexportplugininterface.h"
#include "kaddressbookexportselectionwidget.h"

#include <KContacts/Addressee>

class VCardImportExportPluginInterface : public KAddressBookImportExport::KAddressBookImportExportPluginInterface
{
    Q_OBJECT
public:
    explicit VCardImportExportPluginInterface(QObject *parent = nullptr);
    ~VCardImportExportPluginInterface() override;

    void createAction(KActionCollection *ac) override;
    void exec() override;
    Q_REQUIRED_RESULT bool canImportFileType(const QUrl &url) override;
private:
    enum ExportVCardType {
        VCard2_1 = 0,
        VCard3,
        VCard4
    };
    KContacts::Addressee::List parseVCard(const QByteArray &data) const;
    KContacts::Addressee::List filterContacts(const KContacts::Addressee::List &addrList, KAddressBookImportExport::KAddressBookExportSelectionWidget::ExportFields exportFieldType) const;
    void addKey(KContacts::Addressee &addr, KContacts::Key::Type type) const;

    void exportVCard();
    void importVCard();
    void slotImportVCard();
    void slotExportVCard4();
    void slotExportVCard3();
    Q_REQUIRED_RESULT QString contactFileName(const KContacts::Addressee &contact) const;
    void slotExportVCard2();
    Q_REQUIRED_RESULT bool doExport(const QUrl &url, const QByteArray &data) const;

    ExportVCardType mExportVCardType;
};

#endif // VCARDIMPORTEXPORTPLUGININTERFACE_H
