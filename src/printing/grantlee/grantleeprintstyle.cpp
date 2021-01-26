/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "grantleeprintstyle.h"
#include "importexport/contactfields.h"
#include "printingwizard.h"
#include "printprogress.h"
#include <Akonadi/Contact/GrantleePrint>

#include <KContacts/Addressee>

#include <KLocalizedString>

#include <QFile>
#include <QPrinter>
#include <QTextDocument>

using namespace KABPrinting;

GrantleePrintStyle::GrantleePrintStyle(const QString &themePath, PrintingWizard *parent)
    : PrintStyle(parent)
    , m_themePath(themePath)
{
    QFile previewFile(QString(themePath + QStringLiteral("/preview.png")));
    if (previewFile.exists()) {
        setPreview(previewFile.fileName());
    }
    setPreferredSortOptions(KAddressBookImportExport::ContactFields::FormattedName, Qt::AscendingOrder);
}

GrantleePrintStyle::~GrantleePrintStyle()
{
}

void GrantleePrintStyle::print(const KContacts::Addressee::List &contacts, PrintProgress *progress)
{
    QPrinter *printer = wizard()->printer();
    printer->setPageMargins(QMarginsF(20, 20, 20, 20), QPageLayout::Point);

    progress->addMessage(i18n("Setting up document"));

    KAddressBookGrantlee::GrantleePrint grantleePrint(m_themePath);
    grantleePrint.setApplicationDomain("kaddressbook");
    const QString html = grantleePrint.contactsToHtml(contacts);

    QTextDocument document;
    document.setHtml(html);

    progress->addMessage(i18n("Printing"));

    document.print(printer);

    progress->addMessage(i18nc("Finished printing", "Done"));
}

GrantleeStyleFactory::GrantleeStyleFactory(const QString &name, const QString &themePath, PrintingWizard *parent)
    : PrintStyleFactory(parent)
    , mThemePath(themePath)
    , mName(name)
{
}

PrintStyle *GrantleeStyleFactory::create() const
{
    return new GrantleePrintStyle(mThemePath, mParent);
}

QString GrantleeStyleFactory::description() const
{
    return mName;
}
