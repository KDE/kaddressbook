/*
  This file is part of KAddressBook.
  SPDX-FileCopyrightText: 1996-2002 Mirko Boehm <mirko@kde.org>
  SPDX-FileCopyrightText: 2002 Mike Pilone <mpilone@slac.com>
  SPDX-FileCopyrightText: 2009 Tobias Koenig <tokoe@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later WITH Qt-Commercial-exception-1.0
*/

#include "mikesstyle.h"
#include "importexport/contactfields.h"
#include "printingwizard.h"
#include "printprogress.h"

#include <KContacts/Addressee>

#include <KLocalizedString>

#include <QPrinter>
#include <QTextDocument>

using namespace KABPrinting;
using namespace KAddressBookImportExport;

static QString contactsToHtml(const KContacts::Addressee::List &contacts)
{
    ContactFields::Fields leftFields, rightFields;
    ContactFields::Fields allFields = ContactFields::allFields();
    allFields.remove(0); // drop 'Undefined' field

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
    QString content = QStringLiteral("<html>\n");
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
        content += QLatin1String(
                       "    <th align=\"left\" style=\"color: black;\" bgcolor=\"gray\" "
                       "style=\"padding-left: 20px\" colspan=\"4\">")
            + name + QLatin1String("</th>\n");
        content += QLatin1String("   </tr>\n");

        for (int i = 0; i < max; ++i) {
            QString leftTitle, leftValue, rightTitle, rightValue;

            if (i < leftFields.count()) {
                leftTitle = ContactFields::label(leftFields.at(i)) + QLatin1Char(':');
                leftTitle.replace(QLatin1Char(' '), QStringLiteral("&nbsp;"));
                leftValue = ContactFields::value(leftFields.at(i), contact);
            }

            if (i < rightFields.count()) {
                rightTitle = ContactFields::label(rightFields.at(i)) + QLatin1Char(':');
                rightTitle.replace(QLatin1Char(' '), QStringLiteral("&nbsp;"));
                rightValue = ContactFields::value(rightFields.at(i), contact);
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
    setPreferredSortOptions(ContactFields::FormattedName, Qt::AscendingOrder);
}

MikesStyle::~MikesStyle()
{
}

void MikesStyle::print(const KContacts::Addressee::List &contacts, PrintProgress *progress)
{
    QPrinter *printer = wizard()->printer();
    printer->setPageMargins(QMarginsF(20, 20, 20, 20), QPageLayout::Point);

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
