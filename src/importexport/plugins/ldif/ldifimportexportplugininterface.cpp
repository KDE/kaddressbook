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

#include "ldifimportexportplugininterface.h"
#include "importexportengine.h"
#include "kaddressbookcontactselectiondialog.h"
#include "kaddressbookimportexportcontactlist.h"

#include <KLocalizedString>
#include <KActionCollection>
#include <QAction>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <KMessageBox>
#include <QTemporaryFile>
#include <KJobWidgets>
#include <QPointer>
#include <KIOCore/kio/filecopyjob.h>
#include <PimCommon/RenameFileDialog>
#include <KContacts/LDIFConverter>

LDifImportExportPluginInterface::LDifImportExportPluginInterface(QObject *parent)
    : KAddressBookImportExport::KAddressBookImportExportPluginInterface(parent)
{
}

LDifImportExportPluginInterface::~LDifImportExportPluginInterface()
{
}

void LDifImportExportPluginInterface::createAction(KActionCollection *ac)
{
    QAction *action = ac->addAction(QStringLiteral("file_import_ldif"));
    action->setText(i18n("Import LDIF file..."));
    action->setWhatsThis(i18n("Import contacts from an LDIF file."));
    connect(action, &QAction::triggered, this, &LDifImportExportPluginInterface::slotImportLdif);
    setImportActions(QList<QAction *>() << action);

    action = ac->addAction(QStringLiteral("file_export_ldif"));
    action->setText(i18n("Export LDIF file..."));
    action->setWhatsThis(i18n("Export contacts to an LDIF file."));
    connect(action, &QAction::triggered, this, &LDifImportExportPluginInterface::slotExportLdif);
    setExportActions(QList<QAction *>() << action);
}

void LDifImportExportPluginInterface::exec()
{
    switch (mImportExportAction) {
    case Import:
        importLdif();
        break;
    case Export:
        exportLdif();
        break;
    }
}

void LDifImportExportPluginInterface::slotImportLdif()
{
    mImportExportAction = Import;
    Q_EMIT emitPluginActivated(this);
}

void LDifImportExportPluginInterface::slotExportLdif()
{
    mImportExportAction = Export;
    Q_EMIT emitPluginActivated(this);
}

void LDifImportExportPluginInterface::importLdifFile(const QString &fileName)
{
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        const QString msg = i18n("<qt>Unable to open <b>%1</b> for reading.</qt>", fileName);
        KMessageBox::error(parentWidget(), msg);
        return;
    }

    QTextStream stream(&file);
    stream.setCodec("ISO 8859-1");

    const QString wholeFile = stream.readAll();
    const QDateTime dtDefault = QFileInfo(file).lastModified();
    file.close();
    KContacts::ContactGroup::List lstGroup;
    KContacts::Addressee::List lstAddresses;
    KContacts::LDIFConverter::LDIFToAddressee(wholeFile, lstAddresses, lstGroup, dtDefault);
    KAddressBookImportExport::KAddressBookImportExportContactList contactList;
    contactList.setAddressList(lstAddresses);
    contactList.setContactGroupList(lstGroup);

    ImportExportEngine *engine = new ImportExportEngine(this);
    engine->setContactList(contactList);
    engine->setDefaultAddressBook(defaultCollection());
    engine->importContacts();
}

void LDifImportExportPluginInterface::importLdif()
{
    const QString fileName = QFileDialog::getOpenFileName(parentWidget(), QString(), QDir::homePath(), QStringLiteral("%1 (*.ldif)").arg(i18n("LDif Files")));
    importLdifFile(fileName);
}

void doExport(QFile *file, const KAddressBookImportExport::KAddressBookImportExportContactList &list)
{
    QString data;
    KContacts::LDIFConverter::addresseeAndContactGroupToLDIF(list.addressList(), list.contactGroupList(), data);

    QTextStream stream(file);
    stream.setCodec("UTF-8");
    stream << data;
}

void LDifImportExportPluginInterface::exportLdif()
{
    QPointer<KAddressBookImportExport::KAddressBookContactSelectionDialog> dlg
        = new KAddressBookImportExport::KAddressBookContactSelectionDialog(itemSelectionModel(), false, parentWidget());
    dlg->setMessageText(i18n("Which contact do you want to export?"));
    dlg->setDefaultAddressBook(defaultCollection());
    if (!dlg->exec()) {
        delete dlg;
        return;
    }
    const KContacts::AddresseeList contacts = dlg->selectedContacts().addressList();
    delete dlg;

    if (contacts.isEmpty()) {
        KMessageBox::sorry(nullptr, i18n("You have not selected any contacts to export."));
        return;
    }

    KAddressBookImportExport::KAddressBookImportExportContactList contactLists;
    contactLists.setAddressList(contacts);

    QFileDialog::Options options = QFileDialog::DontConfirmOverwrite;
    const QUrl url
        = QFileDialog::getSaveFileUrl(parentWidget(), QString(), QUrl::fromLocalFile(QDir::homePath() + QStringLiteral("/addressbook.ldif")),
                                      i18n("LDif Files (*.ldif)"), nullptr, options);
    if (url.isEmpty()) {
        return;
    }

    if (!url.isLocalFile()) {
        QTemporaryFile tmpFile;
        if (!tmpFile.open()) {
            const QString msg = i18n("<qt>Unable to open file <b>%1</b></qt>", url.url());
            KMessageBox::error(parentWidget(), msg);
            return;
        }

        doExport(&tmpFile, contactLists);
        tmpFile.flush();
        auto job = KIO::file_copy(QUrl::fromLocalFile(tmpFile.fileName()), url, -1, KIO::Overwrite);
        KJobWidgets::setWindow(job, parentWidget());
        job->exec();
    } else {
        QString fileName = url.toLocalFile();

        if (QFileInfo::exists(fileName)) {
            if (url.isLocalFile() && QFileInfo::exists(url.toLocalFile())) {
                PimCommon::RenameFileDialog *dialog = new PimCommon::RenameFileDialog(url, false, parentWidget());
                PimCommon::RenameFileDialog::RenameFileDialogResult result = static_cast<PimCommon::RenameFileDialog::RenameFileDialogResult>(dialog->exec());
                if (result == PimCommon::RenameFileDialog::RENAMEFILE_RENAME) {
                    fileName = dialog->newName().toLocalFile();
                } else if (result == PimCommon::RenameFileDialog::RENAMEFILE_IGNORE) {
                    delete dialog;
                    return;
                }
                delete dialog;
            }
        }

        QFile file(fileName);

        if (!file.open(QIODevice::WriteOnly)) {
            QString txt = i18n("<qt>Unable to open file <b>%1</b>.</qt>", fileName);
            KMessageBox::error(parentWidget(), txt);
            return;
        }
        QString data;
        KContacts::LDIFConverter::addresseeAndContactGroupToLDIF(contactLists.addressList(), contactLists.contactGroupList(), data);

        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        stream << data;
        file.close();
    }
}

bool LDifImportExportPluginInterface::canImportFileType(const QUrl &url)
{
    return url.path().endsWith(QLatin1String(".ldif"));
}

void LDifImportExportPluginInterface::importFile(const QUrl &url)
{
    if (url.isLocalFile()) {
        importLdifFile(url.path());
    }
}
