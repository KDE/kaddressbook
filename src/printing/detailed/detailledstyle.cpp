/*
  This file is part of KAddressBook.
  SPDX-FileCopyrightText: 1996-2002 Mirko Boehm <mirko@kde.org>
  SPDX-FileCopyrightText: 2009 Tobias Koenig <tokoe@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later WITH Qt-Commercial-exception-1.0
*/

#include "detailledstyle.h"
using namespace Qt::Literals::StringLiterals;

#include "printingwizard.h"
#include "printprogress.h"
#include "printstyle.h"
#include "ui_ds_appearance.h"

#include <KConfig>
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QCheckBox>
#include <QLocale>
#include <QPrinter>
#include <QTextDocument>

using namespace KABPrinting;

static const char ConfigSectionName[] = "DetailedPrintStyle";
static const char ContactHeaderForeColor[] = "ContactHeaderForeColor";
static const char ContactHeaderBGColor[] = "ContactHeaderBGColor";

struct ContactBlock {
    using List = QList<ContactBlock>;

    QString header;
    QStringList entries;
};

struct ColorSettings {
    QString headerTextColor;
    QString headerBackgroundColor;
};

static QString contactsToHtml(const KContacts::Addressee::List &contacts, const ColorSettings &settings)
{
    QString content = u"<html>\n"_s;
    content += QLatin1StringView(" <head>\n");
    content += QLatin1StringView("  <style type=\"text/css\">\n");
    content += QLatin1StringView("    td.indented {\n");
    content += QLatin1StringView("      padding-left: 20px;\n");
    content += QLatin1StringView("      font-family: Fixed, monospace;\n");
    content += QLatin1StringView("    }\n");
    content += QLatin1StringView("  </style>\n");
    content += QLatin1StringView(" </head>\n");
    content += QLatin1StringView(" <body>\n");
    ContactBlock::List blocks;
    for (const KContacts::Addressee &contact : contacts) {
        blocks.clear();
        QString name = contact.realName();
        if (!contact.title().isEmpty() || !contact.role().isEmpty()) {
            QStringList contentAddress;
            if (!contact.title().isEmpty()) {
                contentAddress << contact.title();
            }
            if (!contact.role().isEmpty()) {
                contentAddress << contact.role();
            }
            name += u" (%1)"_s.arg(contentAddress.join(u", "_s));
        }

        const QString birthday = QLocale().toString(contact.birthday().date(), QLocale::ShortFormat);

        if (!contact.organization().isEmpty()) {
            ContactBlock block;
            block.header = i18n("Organization:");
            block.entries.append(contact.organization());

            blocks.append(block);
        }

        if (!contact.emails().isEmpty()) {
            ContactBlock block;
            block.header = (contact.emails().count() == 1 ? i18n("Email address:") : i18n("Email addresses:"));
            block.entries = contact.emails();

            blocks.append(block);
        }

        if (!contact.phoneNumbers().isEmpty()) {
            const KContacts::PhoneNumber::List numbers = contact.phoneNumbers();

            ContactBlock block;
            block.header = (numbers.count() == 1 ? i18n("Telephone:") : i18n("Telephones:"));

            for (const KContacts::PhoneNumber &number : numbers) {
                const QString line = number.typeLabel() + QLatin1StringView(": ") + number.number();
                block.entries.append(line);
            }

            blocks.append(block);
        }

        if (contact.url().isValid()) {
            ContactBlock block;
            block.header = i18n("Web page:");
            block.entries.append(contact.url().url().toDisplayString());

            blocks.append(block);
        }

        if (!contact.addresses().isEmpty()) {
            const KContacts::Address::List addresses = contact.addresses();

            for (const KContacts::Address &address : addresses) {
                ContactBlock block;

                switch (address.type()) {
                case KContacts::Address::Dom:
                    block.header = i18n("Domestic Address");
                    break;
                case KContacts::Address::Intl:
                    block.header = i18n("International Address");
                    break;
                case KContacts::Address::Postal:
                    block.header = i18n("Postal Address");
                    break;
                case KContacts::Address::Parcel:
                    block.header = i18n("Parcel Address");
                    break;
                case KContacts::Address::Home:
                    block.header = i18n("Home Address");
                    break;
                case KContacts::Address::Work:
                    block.header = i18n("Work Address");
                    break;
                case KContacts::Address::Pref:
                default:
                    block.header = i18n("Preferred Address");
                }
                block.header += u':';

                block.entries = address.formatted(KContacts::AddressFormatStyle::Postal).split(u'\n', Qt::KeepEmptyParts);
                blocks.append(block);
            }
        }

        if (!contact.note().isEmpty()) {
            ContactBlock block;
            block.header = i18n("Notes:");
            block.entries = contact.note().split(u'\n', Qt::KeepEmptyParts);

            blocks.append(block);
        }

        // add header
        content += QLatin1StringView(
            "  <table style=\"border-width: 0px; border-spacing: 0px; "
            "page-break-inside: avoid\" cellspacing=\"0\" cellpadding=\"0\" width=\"100%\">\n");
        content += QLatin1StringView("   <tr>\n");
        content += QLatin1StringView("    <td style=\"color: ") + settings.headerTextColor + QLatin1StringView(";\" bgcolor=\"")
            + settings.headerBackgroundColor + QLatin1StringView(R"(" style="padding-left: 20px">)") + name + QLatin1StringView("</td>\n");
        content += QLatin1StringView("    <td style=\"color: ") + settings.headerTextColor + QLatin1StringView(R"(;" align="right" bgcolor=")")
            + settings.headerBackgroundColor + QLatin1StringView(R"(" style="padding-right: 20px">)") + birthday + QLatin1StringView("</td>\n");
        content += QLatin1StringView("   </tr>\n");

        for (int i = 0; i < blocks.count(); i += 2) {
            // add empty line for spacing
            content += QLatin1StringView("   <tr>\n");
            content += QLatin1StringView("    <td>&nbsp;</td>\n");
            content += QLatin1StringView("    <td>&nbsp;</td>\n");
            content += QLatin1StringView("   </tr>\n");

            // add real block data
            const ContactBlock leftBlock = blocks.at(i);
            const ContactBlock rightBlock = ((i + 1 < blocks.count()) ? blocks.at(i + 1) : ContactBlock());

            content += QLatin1StringView("   <tr>\n");
            content += QLatin1StringView("    <td>") + leftBlock.header + QLatin1StringView("</td>\n");
            content += QLatin1StringView("    <td>") + rightBlock.header + QLatin1StringView("</td>\n");
            content += QLatin1StringView("   </tr>\n");

            const int maxLines = qMax(leftBlock.entries.count(), rightBlock.entries.count());
            for (int j = 0; j < maxLines; ++j) {
                QString leftLine;
                QString rightLine;

                if (j < leftBlock.entries.count()) {
                    leftLine = leftBlock.entries.at(j);
                }

                if (j < rightBlock.entries.count()) {
                    rightLine = rightBlock.entries.at(j);
                }

                content += QLatin1StringView("   <tr>\n");
                content += QLatin1StringView("    <td class=\"indented\">") + leftLine + QLatin1StringView("</td>\n");
                content += QLatin1StringView("    <td class=\"indented\">") + rightLine + QLatin1StringView("</td>\n");
                content += QLatin1StringView("   </tr>\n");
            }
        }

        // add empty line for spacing
        content += QLatin1StringView("   <tr>\n");
        content += QLatin1StringView("    <td>&nbsp;</td>\n");
        content += QLatin1StringView("    <td>&nbsp;</td>\n");
        content += QLatin1StringView("   </tr>\n");
        content += QLatin1StringView("  </table>\n");
    }
    content += QLatin1StringView(" </body>\n");
    content += QLatin1StringView("</html>\n");

    return content;
}

class KABPrinting::AppearancePage : public QWidget, public Ui::AppearancePage_Base
{
public:
    explicit AppearancePage(QWidget *parent)
        : QWidget(parent)
    {
        setupUi(this);
        setObjectName(QLatin1StringView("AppearancePage"));
    }
};

DetailledPrintStyle::DetailledPrintStyle(PrintingWizard *parent)
    : PrintStyle(parent)
    , mPageAppearance(new AppearancePage(parent))
{
    setPreview(u"detailed-style.png"_s);
    setPreferredSortOptions(KAddressBookImportExport::ContactFields::FormattedName, Qt::AscendingOrder);

    addPage(mPageAppearance, i18n("Detailed Print Style - Appearance"));

    KConfigGroup config(KSharedConfig::openConfig(), QLatin1StringView(ConfigSectionName));

    mPageAppearance->kcbHeaderBGColor->setColor(config.readEntry(ContactHeaderBGColor, QColor(Qt::black)));

    mPageAppearance->kcbHeaderTextColor->setColor(config.readEntry(ContactHeaderForeColor, QColor(Qt::white)));
}

DetailledPrintStyle::~DetailledPrintStyle() = default;

void DetailledPrintStyle::print(const KContacts::Addressee::List &contacts, PrintProgress *progress)
{
    progress->addMessage(i18n("Setting up colors"));
    progress->setProgress(0);

    const QColor headerBackgroundColor = mPageAppearance->kcbHeaderBGColor->color();
    const QColor headerForegroundColor = mPageAppearance->kcbHeaderTextColor->color();

    KConfigGroup config(KSharedConfig::openConfig(), QLatin1StringView(ConfigSectionName));
    config.writeEntry(ContactHeaderForeColor, headerForegroundColor);
    config.writeEntry(ContactHeaderBGColor, headerBackgroundColor);
    config.sync();

    ColorSettings settings;
    settings.headerBackgroundColor = headerBackgroundColor.name();
    settings.headerTextColor = headerForegroundColor.name();

    QPrinter *printer = wizard()->printer();

    progress->addMessage(i18n("Setting up document"));

    const QString html = contactsToHtml(contacts, settings);

    QTextDocument document;
    document.setHtml(html);

    progress->addMessage(i18n("Printing"));

    document.print(printer);

    progress->addMessage(i18nc("Finished printing", "Done"));
}

DetailledPrintStyleFactory::DetailledPrintStyleFactory(PrintingWizard *parent)
    : PrintStyleFactory(parent)
{
}

PrintStyle *DetailledPrintStyleFactory::create() const
{
    return new DetailledPrintStyle(mParent);
}

QString DetailledPrintStyleFactory::description() const
{
    return i18n("Detailed Style");
}

#include "moc_detailledstyle.cpp"
