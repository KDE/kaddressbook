/*
  This file is part of KAddressBook.
  Copyright (c) 1996-2002 Mirko Boehm <mirko@kde.org>
                     2002 Mike Pilone <mpilone@slac.com>
                     2009 Tobias Koenig <tokoe@kde.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

  As a special exception, permission is given to link this program
  with any edition of Qt, and distribute the resulting executable,
  without including the source code for Qt in the source distribution.
*/

#include "mikesstyle.h"
#include "kaddressbookimportexportcontactfields.h"
#include "printingwizard.h"
#include "printprogress.h"
#include "printstyle.h"

#include <KContacts/Addressee>

#include <KLocalizedString>

#include <QPrinter>
#include <QTextDocument>

using namespace KABPrinting;

static QString contactsToHtml(const KContacts::Addressee::List &contacts)
{

    KAddressBookImportExport::KAddressBookImportExportContactFields::Fields leftFields, rightFields;
    KAddressBookImportExport::KAddressBookImportExportContactFields::Fields allFields = KAddressBookImportExport::KAddressBookImportExportContactFields::allFields();
    allFields.remove(0);   // drop 'Undefined' field

    const int middle = allFields.count() / 2;

    leftFields.reserve(middle);
    for (int i = 0; i < middle; ++i) {
        leftFields.append(allFields.at(i));
    }

    rightFields.reserve(allFields.count() - middle);
    for (int i = middle; i < allFields.count(); ++i) {
        rightFields.append(allFields.at(i));
    }

    int counter = 0;
    QString content = QLatin1String("<html>\n");
    content += QLatin1String(" <body>\n");
    for (const KContacts::Addressee &contact : contacts) {
        const int max = qMax(leftFields.count(), rightFields.count());

        const QString name = contact.realName();

        if (counter % 2) {
            content += QLatin1String("  <br/><br/>\n");
        }

        // start a new page after every second table
        const QString pageBreak = ((counter % 2) ? QStringLiteral("page-break-after: always;") : QString());

        content += QLatin1String("  <table style=\"border-width: 0px; ") + pageBreak + QLatin1String("\" width=\"100%\">\n");
        content += QLatin1String("   <tr>\n");
        content += QLatin1String("    <th align=\"left\" style=\"color: black;\" bgcolor=\"gray\" "
                                 "style=\"padding-left: 20px\" colspan=\"4\">") + name + QLatin1String("</th>\n");
        content += QLatin1String("   </tr>\n");

        for (int i = 0; i < max; ++i) {
            QString leftTitle, leftValue, rightTitle, rightValue;

            if (i < leftFields.count()) {
                leftTitle = KAddressBookImportExport::KAddressBookImportExportContactFields::label(leftFields.at(i)) + QLatin1Char(':');
                leftTitle = leftTitle.replace(QLatin1Char(' '), QStringLiteral("&nbsp;"));
                leftValue = KAddressBookImportExport::KAddressBookImportExportContactFields::value(leftFields.at(i), contact);
            }

            if (i < rightFields.count()) {
                rightTitle = KAddressBookImportExport::KAddressBookImportExportContactFields::label(rightFields.at(i)) + QLatin1Char(':');
                rightTitle = rightTitle.replace(QLatin1Char(' '), QStringLiteral("&nbsp;"));
                rightValue = KAddressBookImportExport::KAddressBookImportExportContactFields::value(rightFields.at(i), contact);
            }

            content += QLatin1String("   <tr>\n");
            content += QLatin1String("    <td>") + leftTitle + QLatin1String("</td>\n");
            content += QLatin1String("    <td>") + leftValue + QLatin1String("</td>\n");
            content += QLatin1String("    <td>") + rightTitle + QLatin1String("</td>\n");
            content += QLatin1String("    <td>") + rightValue + QLatin1String("</td>\n");
            content += QLatin1String("   </tr>\n");
        }
        content += QLatin1String("  </table>\n");

        counter++;
    }
    content += QLatin1String(" </body>\n");
    content += QLatin1String("</html>\n");

    return content;
}

MikesStyle::MikesStyle(PrintingWizard *parent)
    : PrintStyle(parent)
{
    setPreview(QStringLiteral("mike-style.png"));
    setPreferredSortOptions(KAddressBookImportExport::KAddressBookImportExportContactFields::FormattedName, Qt::AscendingOrder);
}

MikesStyle::~MikesStyle()
{
}

void MikesStyle::print(const KContacts::Addressee::List &contacts, PrintProgress *progress)
{
    QPrinter *printer = wizard()->printer();
    printer->setPageMargins(20, 20, 20, 20, QPrinter::DevicePixel);

    progress->addMessage(i18n("Setting up document"));

    const QString html = contactsToHtml(contacts);

    QTextDocument document;
    document.setHtml(html);

    progress->addMessage(i18n("Printing"));

    document.print(printer);

    progress->addMessage(i18nc("Finished printing", "Done"));
}

MikesStyleFactory::MikesStyleFactory(PrintingWizard *parent)
    : PrintStyleFactory(parent)
{
}

PrintStyle *MikesStyleFactory::create() const
{
    return new MikesStyle(mParent);
}

QString MikesStyleFactory::description() const
{
    return i18n("Mike's Printing Style");
}

