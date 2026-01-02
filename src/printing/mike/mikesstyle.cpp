/*
  This file is part of KAddressBook.
  SPDX-FileCopyrightText: 1996-2002 Mirko Boehm <mirko@kde.org>
  SPDX-FileCopyrightText: 2002 Mike Pilone <mpilone@slack.com>
  SPDX-FileCopyrightText: 2009 Tobias Koenig <tokoe@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later WITH LicenseRef-Qt-Commercial-exception-1.0
*/

#include "mikesstyle.h"
using namespace Qt::Literals::StringLiterals;

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
    ContactFields::Fields leftFields;
    ContactFields::Fields rightFields;
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
    QString content = u"<html>\n"_s;
    content += QLatin1StringView(" <body>\n");
    for (const KContacts::Addressee &contact : contacts) {
        const int max = qMax(leftFields.count(), rightFields.count());

        const QString name = contact.realName();

        if (counter % 2) {
            content += QLatin1StringView("  <br/><br/>\n");
        }

        // start a new page after every second table
        const QString pageBreak = ((counter % 2) ? u"page-break-after: always;"_s : QString());

        content += QLatin1StringView("  <table style=\"border-width: 0px; ") + pageBreak + QLatin1StringView("\" width=\"100%\">\n");
        content += QLatin1StringView("   <tr>\n");
        content += QLatin1StringView(
                       "    <th align=\"left\" style=\"color: black;\" bgcolor=\"gray\" "
                       "style=\"padding-left: 20px\" colspan=\"4\">")
            + name + QLatin1StringView("</th>\n");
        content += QLatin1StringView("   </tr>\n");

        for (int i = 0; i < max; ++i) {
            QString leftTitle;
            QString leftValue;
            QString rightTitle;
            QString rightValue;

            if (i < leftFields.count()) {
                leftTitle = ContactFields::label(leftFields.at(i)) + u':';
                leftTitle.replace(u' ', u"&nbsp;"_s);
                leftValue = ContactFields::value(leftFields.at(i), contact);
            }

            if (i < rightFields.count()) {
                rightTitle = ContactFields::label(rightFields.at(i)) + u':';
                rightTitle.replace(u' ', u"&nbsp;"_s);
                rightValue = ContactFields::value(rightFields.at(i), contact);
            }

            content += QLatin1StringView("   <tr>\n");
            content += QLatin1StringView("    <td>") + leftTitle + QLatin1StringView("</td>\n");
            content += QLatin1StringView("    <td>") + leftValue + QLatin1StringView("</td>\n");
            content += QLatin1StringView("    <td>") + rightTitle + QLatin1StringView("</td>\n");
            content += QLatin1StringView("    <td>") + rightValue + QLatin1StringView("</td>\n");
            content += QLatin1StringView("   </tr>\n");
        }
        content += QLatin1StringView("  </table>\n");

        counter++;
    }
    content += QLatin1StringView(" </body>\n");
    content += QLatin1StringView("</html>\n");

    return content;
}

MikesStyle::MikesStyle(PrintingWizard *parent)
    : PrintStyle(parent)
{
    setPreview(u"mike-style.png"_s);
    setPreferredSortOptions(ContactFields::FormattedName, Qt::AscendingOrder);
}

MikesStyle::~MikesStyle() = default;

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

#include "moc_mikesstyle.cpp"
