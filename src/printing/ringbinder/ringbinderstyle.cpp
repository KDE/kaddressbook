/*
  This file is part of KAddressBook.
  SPDX-FileCopyrightText: 2002 Jost Schenck <jost@schenck.de>
  SPDX-FileCopyrightText: 2009 Tobias Koenig <tokoe@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later WITH Qt-Commercial-exception-1.0
*/

#include "ringbinderstyle.h"
#include "printingwizard.h"
#include "printprogress.h"
#include "ui_rbs_appearance.h"

#include <KConfig>
#include <KConfigGroup>
#include <KLocalizedString>

#include <KSharedConfig>
#include <QLocale>
#include <QPrinter>
#include <QTextDocument>

using namespace KABPrinting;

static const char RingBinderConfigSectionName[] = "RingBinderPrintStyle";
static const char ShowPhoneNumbers[] = "ShowPhoneNumbers";
static const char ShowEmailAddresses[] = "ShowEmailAddresses";
static const char ShowStreetAddresses[] = "ShowStreetAddresses";
static const char ShowOrganization[] = "ShowOrganization";
static const char ShowBirthday[] = "ShowBirthday";
static const char ShowNote[] = "ShowNote";

enum PrintField { PhoneNumbers = 1, Emails = 2, Addresses = 4, Organization = 8, Birthday = 16, Note = 32 };

static QString contactsToHtml(const KContacts::Addressee::List &contacts, int fields)
{
    QString content = QStringLiteral("<html>\n");
    content += QLatin1String(" <body>\n");
    content += QLatin1String(
        "  <table style=\"border-width: 1px; border-style: solid; "
        "border-color: gray;\" width=\"100%\" cellspacing=\"0\">\n");
    for (const KContacts::Addressee &contact : contacts) {
        QString nameString = contact.familyName() + QLatin1String(", ") + contact.givenName();

        if (fields & Organization) {
            if (!contact.organization().isEmpty()) {
                nameString += QLatin1String(" (") + contact.organization() + QLatin1Char(')');
            }
        }

        if (fields & Birthday) {
            if (contact.birthday().isValid()) {
                nameString += QLatin1String(" *") + QLocale().toString(contact.birthday().date(), QLocale::ShortFormat);
            }
        }

        QStringList leftBlock, rightBlock;
        if (fields & PhoneNumbers) {
            const KContacts::PhoneNumber::List numbers = contact.phoneNumbers();
            for (const KContacts::PhoneNumber &number : numbers) {
                rightBlock.append(number.typeLabel() + QLatin1String(": ") + number.number());
            }
        }
        if (fields & Emails) {
            const QStringList emails = contact.emails();
            for (const QString &email : emails) {
                rightBlock.append(email);
            }
        }
        if (fields & Note) {
            if (!contact.note().isEmpty()) {
                const QString note = i18n("Note: ") + contact.note().replace(QLatin1Char('\n'), QStringLiteral("<br/>"));

                rightBlock.append(note);
            }
        }
        if (fields & Addresses) {
            const KContacts::Address::List addresses = contact.addresses();
            for (const KContacts::Address &address : addresses) {
                const QString data =
                    address.formattedAddress().replace(QLatin1String("\n\n"), QStringLiteral("\n")).replace(QLatin1Char('\n'), QStringLiteral("<br/>"));
                const QString subBlock = QLatin1String("<p style=\"margin-top: 0px; margin-left: 20px\">") + data + QLatin1String("</p>");

                leftBlock.append(subBlock);
            }
        }

        content += QLatin1String("   <tr>\n");
        content += QLatin1String(
                       "    <td style=\"padding-left: 3px; padding-top: 3px; padding-right: 3px; "
                       "padding-bottom: 3px;\">")
            + nameString + leftBlock.join(QString()) + QLatin1String("</td>\n");
        content += QLatin1String(
                       "    <td style=\"padding-left: 3px; padding-top: 3px; padding-right: 3px; "
                       "padding-bottom: 3px;\">")
            + rightBlock.join(QLatin1String("<br/>")) + QLatin1String("</td>\n");
        content += QLatin1String("   </tr>\n");
    }
    content += QLatin1String("  </table>\n");
    content += QLatin1String(" </body>\n");
    content += QLatin1String("</html>\n");

    return content;
}

namespace KABPrinting
{
class RingBinderStyleAppearanceForm : public QWidget, public Ui::RingBinderStyleAppearanceForm_Base
{
public:
    explicit RingBinderStyleAppearanceForm(QWidget *parent)
        : QWidget(parent)
    {
        setObjectName(QStringLiteral("AppearancePage"));
        setupUi(this);
    }
};
}

RingBinderPrintStyle::RingBinderPrintStyle(PrintingWizard *parent)
    : PrintStyle(parent)
    , mPageAppearance(new RingBinderStyleAppearanceForm(parent))
{
    setPreview(QStringLiteral("ringbinder-style.png"));
    setPreferredSortOptions(KAddressBookImportExport::ContactFields::FamilyName, Qt::AscendingOrder);

    addPage(mPageAppearance, i18n("Ring Binder Printing Style - Appearance"));

    // applying previous settings
    KConfigGroup config(KSharedConfig::openConfig(), RingBinderConfigSectionName);
    mPageAppearance->cbPhoneNumbers->setChecked(config.readEntry(ShowPhoneNumbers, true));
    mPageAppearance->cbEmails->setChecked(config.readEntry(ShowEmailAddresses, true));
    mPageAppearance->cbStreetAddresses->setChecked(config.readEntry(ShowStreetAddresses, true));
    mPageAppearance->cbOrganization->setChecked(config.readEntry(ShowOrganization, true));
    mPageAppearance->cbBirthday->setChecked(config.readEntry(ShowBirthday, false));
    mPageAppearance->cbNote->setChecked(config.readEntry(ShowNote, false));
}

RingBinderPrintStyle::~RingBinderPrintStyle()
{
}

void RingBinderPrintStyle::print(const KContacts::Addressee::List &contacts, PrintProgress *progress)
{
    progress->addMessage(i18n("Setting up fields"));
    progress->setProgress(0);

    // first write current config settings
    KConfigGroup config(KSharedConfig::openConfig(), RingBinderConfigSectionName);
    config.writeEntry(ShowPhoneNumbers, mPageAppearance->cbPhoneNumbers->isChecked());
    config.writeEntry(ShowEmailAddresses, mPageAppearance->cbEmails->isChecked());
    config.writeEntry(ShowStreetAddresses, mPageAppearance->cbStreetAddresses->isChecked());
    config.writeEntry(ShowOrganization, mPageAppearance->cbOrganization->isChecked());
    config.writeEntry(ShowBirthday, mPageAppearance->cbBirthday->isChecked());
    config.writeEntry(ShowNote, mPageAppearance->cbNote->isChecked());
    config.sync();

    QPrinter *printer = wizard()->printer();
    printer->setPageMargins(QMarginsF(50, 20, 0, 50), QPageLayout::Point);

    progress->addMessage(i18n("Setting up document"));

    int fields = 0;

    if (mPageAppearance->cbPhoneNumbers->isChecked()) {
        fields |= PhoneNumbers;
    }

    if (mPageAppearance->cbEmails->isChecked()) {
        fields |= Emails;
    }

    if (mPageAppearance->cbStreetAddresses->isChecked()) {
        fields |= Addresses;
    }

    if (mPageAppearance->cbOrganization->isChecked()) {
        fields |= Organization;
    }

    if (mPageAppearance->cbBirthday->isChecked()) {
        fields |= Birthday;
    }

    if (mPageAppearance->cbNote->isChecked()) {
        fields |= Note;
    }

    const QString html = contactsToHtml(contacts, fields);

    QTextDocument document;
    document.setHtml(html);

    progress->addMessage(i18n("Printing"));

    document.print(printer);

    progress->addMessage(i18nc("Finished printing", "Done"));
}

RingBinderPrintStyleFactory::RingBinderPrintStyleFactory(PrintingWizard *parent)
    : PrintStyleFactory(parent)
{
}

PrintStyle *RingBinderPrintStyleFactory::create() const
{
    return new RingBinderPrintStyle(mParent);
}

QString RingBinderPrintStyleFactory::description() const
{
    return i18n("Printout for Ring Binders");
}
