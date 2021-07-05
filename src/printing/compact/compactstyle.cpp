/*
  This file is part of KAddressBook.
  SPDX-FileCopyrightText: 2011 Mario Scheel <zweistein12@gmx.de>

  SPDX-License-Identifier: GPL-2.0-or-later WITH Qt-Commercial-exception-1.0
*/

#include "compactstyle.h"
#include "importexport/contactfields.h"
#include "printprogress.h"

#include "printingwizard.h"
#include "ui_compactstyle.h"

#include <KContacts/Addressee>

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QCheckBox>
#include <QPrinter>
#include <QTextDocument>

using namespace KABPrinting;
using namespace KAddressBookImportExport;

const char *CompactStyleConfigSectionName = "CompactStyle";
const char *WithAlternating = "WithAlternating";
const char *WithHomeAddress = "WithHomeAddress";
const char *WithBusinessAddress = "WithBusinessAddress";
const char *WithBirthday = "WithBirthday";
const char *WithEMail = "WithEMail";
const char *FirstColor = "FirstColor";
const char *SecondColor = "SecondColor";

namespace KABPrinting
{
class CompactStyleForm : public QWidget, public Ui::CompactStyleForm_Base
{
public:
    explicit CompactStyleForm(QWidget *parent)
        : QWidget(parent)
    {
        setObjectName(QStringLiteral("AppearancePage"));
        setupUi(this);
    }
};
}

QString CompactStyle::contactsToHtml(const KContacts::Addressee::List &contacts) const
{
    // collect the fields are need to print
    ContactFields::Fields fields;
    fields << ContactFields::FormattedName;
    if (this->withHomeAddress) {
        fields << ContactFields::HomeAddressStreet << ContactFields::HomeAddressPostalCode << ContactFields::HomeAddressLocality << ContactFields::HomePhone
               << ContactFields::MobilePhone;
    }
    if (this->withBusinessAddress) {
        fields << ContactFields::BusinessAddressStreet << ContactFields::BusinessAddressPostalCode << ContactFields::BusinessAddressLocality
               << ContactFields::BusinessPhone;
    }
    if (this->withEMail) {
        fields << ContactFields::PreferredEmail << ContactFields::Email2;
    }
    if (this->withBirthday) {
        fields << ContactFields::Birthday;
    }

    QString content = QStringLiteral("<html>\n");
    content += QLatin1String(" <body>\n");
    content += QLatin1String("  <table style=\"font-size:50%; border-width: 0px; \"width=\"100%\">\n");

    bool odd = false;
    for (const KContacts::Addressee &contact : contacts) {
        // get the values
        QStringList values;
        for (const ContactFields::Field &field : std::as_const(fields)) {
            // we need only values with content
            const QString value = ContactFields::value(field, contact).trimmed();
            if (!value.isEmpty()) {
                values << value;
            }
        }

        content += QLatin1String("   <tr>\n");
        QString style = QStringLiteral("background-color:");
        if (this->withAlternating) {
            style += (odd) ? this->firstColor.name() : this->secondColor.name();
        } else {
            style += QLatin1String("#ffffff");
        }
        content += QLatin1String("    <td style=\"") + style + QLatin1String(";\">") + values.join(QLatin1String("; ")) + QLatin1String("</td>\n");
        content += QLatin1String("   </tr>\n");
        odd = !odd;
    }

    content += QLatin1String("  </table>\n");
    content += QLatin1String(" </body>\n");
    content += QLatin1String("</html>\n");

    return content;
}

CompactStyle::CompactStyle(PrintingWizard *parent)
    : PrintStyle(parent)
    , mPageSettings(new CompactStyleForm(parent))
{
    setPreview(QStringLiteral("compact-style.png"));
    setPreferredSortOptions(ContactFields::FormattedName, Qt::AscendingOrder);

    addPage(mPageSettings, i18n("Compact Style"));

    connect(mPageSettings->cbAlternating, &QCheckBox::clicked, this, &CompactStyle::setAlternatingColors);

    // set the controls, with the values in config
    KConfigGroup config(KSharedConfig::openConfig(), CompactStyleConfigSectionName);

    withAlternating = config.readEntry(WithAlternating, true);
    withHomeAddress = config.readEntry(WithHomeAddress, true);
    withBusinessAddress = config.readEntry(WithBusinessAddress, false);
    withBirthday = config.readEntry(WithBirthday, true);
    withEMail = config.readEntry(WithEMail, true);

    mPageSettings->cbFirst->setColor(config.readEntry(FirstColor, QColor(220, 220, 220)));
    mPageSettings->cbSecond->setColor(config.readEntry(SecondColor, QColor(255, 255, 255)));
    mPageSettings->cbAlternating->setChecked(withAlternating);
    mPageSettings->cbWithHomeAddress->setChecked(withHomeAddress);
    mPageSettings->cbWithBusinessAddress->setChecked(withBusinessAddress);
    mPageSettings->cbWithBirthday->setChecked(withBirthday);
    mPageSettings->cbWithEMail->setChecked(withEMail);

    // set up the color boxes
    setAlternatingColors();
}

CompactStyle::~CompactStyle()
{
}

void CompactStyle::print(const KContacts::Addressee::List &contacts, PrintProgress *progress)
{
    // from UI to members
    withAlternating = mPageSettings->cbAlternating->isChecked();
    firstColor = mPageSettings->cbFirst->color();
    secondColor = mPageSettings->cbSecond->color();
    withHomeAddress = mPageSettings->cbWithHomeAddress->isChecked();
    withBusinessAddress = mPageSettings->cbWithBusinessAddress->isChecked();
    withBirthday = mPageSettings->cbWithBirthday->isChecked();
    withEMail = mPageSettings->cbWithEMail->isChecked();

    // to config
    KConfigGroup config(KSharedConfig::openConfig(), CompactStyleConfigSectionName);

    config.writeEntry(WithAlternating, withAlternating);
    config.writeEntry(FirstColor, firstColor);
    config.writeEntry(SecondColor, secondColor);
    config.writeEntry(WithHomeAddress, withHomeAddress);
    config.writeEntry(WithBusinessAddress, withBusinessAddress);
    config.writeEntry(WithBirthday, withBirthday);
    config.writeEntry(WithEMail, withEMail);
    config.sync();

    // print
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

void CompactStyle::setAlternatingColors()
{
    mPageSettings->cbFirst->setEnabled(mPageSettings->cbAlternating->isChecked());
    mPageSettings->lbCbFirst->setEnabled(mPageSettings->cbAlternating->isChecked());
    mPageSettings->cbSecond->setEnabled(mPageSettings->cbAlternating->isChecked());
    mPageSettings->lbCbSecond->setEnabled(mPageSettings->cbAlternating->isChecked());
}

CompactStyleFactory::CompactStyleFactory(PrintingWizard *parent)
    : PrintStyleFactory(parent)
{
}

PrintStyle *CompactStyleFactory::create() const
{
    return new CompactStyle(mParent);
}

QString CompactStyleFactory::description() const
{
    return i18n("Compact Printing Style");
}
