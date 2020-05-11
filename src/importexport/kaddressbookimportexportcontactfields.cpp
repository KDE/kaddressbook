/*
  This file is part of KAddressBook.

  Copyright (c) 2009 Tobias Koenig <tokoe@kde.org>

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
*/

#include "kaddressbookimportexportcontactfields.h"

#include <KLocalizedString>
#include <QUrl>
using namespace KAddressBookImportExport;

QString KAddressBookImportExportContactFields::label(Field field)
{
    switch (field) {
    case Undefined:
        return i18nc("@item Undefined import field type", "Undefined");
        break;
    case FormattedName:
        return KContacts::Addressee::formattedNameLabel();
    case Prefix:
        return KContacts::Addressee::prefixLabel();
    case GivenName:
        return KContacts::Addressee::givenNameLabel();
    case AdditionalName:
        return KContacts::Addressee::additionalNameLabel();
    case FamilyName:
        return KContacts::Addressee::familyNameLabel();
    case Suffix:
        return KContacts::Addressee::suffixLabel();
    case NickName:
        return KContacts::Addressee::nickNameLabel();
    case Birthday:
        return KContacts::Addressee::birthdayLabel();
    case Anniversary:
        return i18nc("The wedding anniversary of a contact", "Anniversary");
    case HomeAddressStreet:
        return KContacts::Addressee::homeAddressStreetLabel();
    case HomeAddressPostOfficeBox:
        return KContacts::Addressee::homeAddressPostOfficeBoxLabel();
    case HomeAddressLocality:
        return KContacts::Addressee::homeAddressLocalityLabel();
    case HomeAddressRegion:
        return KContacts::Addressee::homeAddressRegionLabel();
    case HomeAddressPostalCode:
        return KContacts::Addressee::homeAddressPostalCodeLabel();
    case HomeAddressCountry:
        return KContacts::Addressee::homeAddressCountryLabel();
    case HomeAddressLabel:
        return KContacts::Addressee::homeAddressLabelLabel();
    case BusinessAddressStreet:
        return KContacts::Addressee::businessAddressStreetLabel();
    case BusinessAddressPostOfficeBox:
        return KContacts::Addressee::businessAddressPostOfficeBoxLabel();
    case BusinessAddressLocality:
        return KContacts::Addressee::businessAddressLocalityLabel();
    case BusinessAddressRegion:
        return KContacts::Addressee::businessAddressRegionLabel();
    case BusinessAddressPostalCode:
        return KContacts::Addressee::businessAddressPostalCodeLabel();
    case BusinessAddressCountry:
        return KContacts::Addressee::businessAddressCountryLabel();
    case BusinessAddressLabel:
        return KContacts::Addressee::businessAddressLabelLabel();
    case HomePhone:
        return KContacts::Addressee::homePhoneLabel();
    case BusinessPhone:
        return KContacts::Addressee::businessPhoneLabel();
    case MobilePhone:
        return KContacts::Addressee::mobilePhoneLabel();
    case HomeFax:
        return KContacts::Addressee::homeFaxLabel();
    case BusinessFax:
        return KContacts::Addressee::businessFaxLabel();
    case CarPhone:
        return KContacts::Addressee::carPhoneLabel();
    case Isdn:
        return KContacts::Addressee::isdnLabel();
    case Pager:
        return KContacts::Addressee::pagerLabel();
    case PreferredEmail:
        return i18nc("Preferred email address", "EMail (preferred)");
    case Email2:
        return i18nc("Second email address", "EMail (2)");
    case Email3:
        return i18nc("Third email address", "EMail (3)");
    case Email4:
        return i18nc("Fourth email address", "EMail (4)");
    case Mailer:
        return KContacts::Addressee::mailerLabel();
    case Title:
        return KContacts::Addressee::titleLabel();
    case Role:
        return KContacts::Addressee::roleLabel();
    case Organization:
        return KContacts::Addressee::organizationLabel();
    case Note:
        return KContacts::Addressee::noteLabel();
    case Homepage:
        return KContacts::Addressee::urlLabel();
    case BlogFeed:
        return i18n("Blog Feed");
    case Profession:
        return i18n("Profession");
    case Office:
        return i18n("Office");
    case Manager:
        return i18n("Manager");
    case Assistant:
        return i18n("Assistant");
    case Spouse:
        return i18n("Spouse");
    }

    return QString();
}

KAddressBookImportExportContactFields::Fields KAddressBookImportExportContactFields::allFields()
{
    Fields fields;

    fields << Undefined
           << FormattedName
           << Prefix
           << GivenName
           << AdditionalName
           << FamilyName
           << Suffix
           << NickName
           << Birthday
           << Anniversary
           << PreferredEmail
           << Email2
           << Email3
           << Email4
           << HomeAddressStreet
           << HomeAddressPostOfficeBox
           << HomeAddressLocality
           << HomeAddressRegion
           << HomeAddressPostalCode
           << HomeAddressCountry
           << HomeAddressLabel
           << BusinessAddressStreet
           << BusinessAddressPostOfficeBox
           << BusinessAddressLocality
           << BusinessAddressRegion
           << BusinessAddressPostalCode
           << BusinessAddressCountry
           << BusinessAddressLabel
           << HomePhone
           << BusinessPhone
           << MobilePhone
           << HomeFax
           << BusinessFax
           << CarPhone
           << Isdn
           << Pager
           << Mailer
           << Title
           << Role
           << Organization
           << Note
           << Homepage
           << BlogFeed
           << Profession
           << Office
           << Manager
           << Assistant
           << Spouse;

    return fields;
}

void KAddressBookImportExportContactFields::setValue(Field field, const QString &value, KContacts::Addressee &contact)
{
    switch (field) {
    case KAddressBookImportExportContactFields::Undefined:
        break;
    case KAddressBookImportExportContactFields::FormattedName:
        contact.setFormattedName(value);
        break;
    case KAddressBookImportExportContactFields::GivenName:
        contact.setGivenName(value);
        break;
    case KAddressBookImportExportContactFields::FamilyName:
        contact.setFamilyName(value);
        break;
    case KAddressBookImportExportContactFields::AdditionalName:
        contact.setAdditionalName(value);
        break;
    case KAddressBookImportExportContactFields::Prefix:
        contact.setPrefix(value);
        break;
    case KAddressBookImportExportContactFields::Suffix:
        contact.setSuffix(value);
        break;
    case KAddressBookImportExportContactFields::NickName:
        contact.setNickName(value);
        break;
    case KAddressBookImportExportContactFields::Birthday:
        contact.setBirthday(QDate::fromString(value, Qt::ISODate));
        break;
    case KAddressBookImportExportContactFields::Anniversary:
        contact.insertCustom(QStringLiteral("KADDRESSBOOK"),
                             QStringLiteral("X-Anniversary"), value);
        break;
    case KAddressBookImportExportContactFields::PreferredEmail:
        contact.insertEmail(value, true);
        break;
    case KAddressBookImportExportContactFields::Email2:
        contact.insertEmail(value, false);
        break;
    case KAddressBookImportExportContactFields::Email3:
        contact.insertEmail(value, false);
        break;
    case KAddressBookImportExportContactFields::Email4:
        contact.insertEmail(value, false);
        break;
    case KAddressBookImportExportContactFields::Role:
        contact.setRole(value);
        break;
    case KAddressBookImportExportContactFields::Title:
        contact.setTitle(value);
        break;
    case KAddressBookImportExportContactFields::Mailer:
        contact.setMailer(value);
        break;
    case KAddressBookImportExportContactFields::Homepage:
    {
        KContacts::ResourceLocatorUrl url;
        url.setUrl(QUrl(value));
        contact.setUrl(url);
        break;
    }
    case KAddressBookImportExportContactFields::Organization:
        contact.setOrganization(value);
        break;
    case KAddressBookImportExportContactFields::Note:
        contact.setNote(value);
        break;
    case KAddressBookImportExportContactFields::HomePhone:
    {
        KContacts::PhoneNumber number = contact.phoneNumber(KContacts::PhoneNumber::Home);
        number.setNumber(value);
        contact.insertPhoneNumber(number);
        break;
    }
    case KAddressBookImportExportContactFields::BusinessPhone:
    {
        KContacts::PhoneNumber number = contact.phoneNumber(KContacts::PhoneNumber::Work);
        number.setNumber(value);
        contact.insertPhoneNumber(number);
        break;
    }
    case KAddressBookImportExportContactFields::MobilePhone:
    {
        KContacts::PhoneNumber number = contact.phoneNumber(KContacts::PhoneNumber::Cell);
        number.setNumber(value);
        contact.insertPhoneNumber(number);
        break;
    }
    case KAddressBookImportExportContactFields::HomeFax:
    {
        KContacts::PhoneNumber number = contact.phoneNumber(KContacts::PhoneNumber::Home
                                                            |KContacts::PhoneNumber::Fax);
        number.setNumber(value);
        contact.insertPhoneNumber(number);
        break;
    }
    case KAddressBookImportExportContactFields::BusinessFax:
    {
        KContacts::PhoneNumber number = contact.phoneNumber(KContacts::PhoneNumber::Work
                                                            |KContacts::PhoneNumber::Fax);
        number.setNumber(value);
        contact.insertPhoneNumber(number);
        break;
    }
    case KAddressBookImportExportContactFields::CarPhone:
    {
        KContacts::PhoneNumber number = contact.phoneNumber(KContacts::PhoneNumber::Car);
        number.setNumber(value);
        contact.insertPhoneNumber(number);
        break;
    }
    case KAddressBookImportExportContactFields::Isdn:
    {
        KContacts::PhoneNumber number = contact.phoneNumber(KContacts::PhoneNumber::Isdn);
        number.setNumber(value);
        contact.insertPhoneNumber(number);
        break;
    }
    case KAddressBookImportExportContactFields::Pager:
    {
        KContacts::PhoneNumber number = contact.phoneNumber(KContacts::PhoneNumber::Pager);
        number.setNumber(value);
        contact.insertPhoneNumber(number);
        break;
    }

    case KAddressBookImportExportContactFields::HomeAddressStreet:
    {
        KContacts::Address address = contact.address(KContacts::Address::Home);
        address.setStreet(value);
        contact.insertAddress(address);
        break;
    }
    case KAddressBookImportExportContactFields::HomeAddressPostOfficeBox:
    {
        KContacts::Address address = contact.address(KContacts::Address::Home);
        address.setPostOfficeBox(value);
        contact.insertAddress(address);
        break;
    }
    case KAddressBookImportExportContactFields::HomeAddressLocality:
    {
        KContacts::Address address = contact.address(KContacts::Address::Home);
        address.setLocality(value);
        contact.insertAddress(address);
        break;
    }
    case KAddressBookImportExportContactFields::HomeAddressRegion:
    {
        KContacts::Address address = contact.address(KContacts::Address::Home);
        address.setRegion(value);
        contact.insertAddress(address);
        break;
    }
    case KAddressBookImportExportContactFields::HomeAddressPostalCode:
    {
        KContacts::Address address = contact.address(KContacts::Address::Home);
        address.setPostalCode(value);
        contact.insertAddress(address);
        break;
    }
    case KAddressBookImportExportContactFields::HomeAddressCountry:
    {
        KContacts::Address address = contact.address(KContacts::Address::Home);
        address.setCountry(value);
        contact.insertAddress(address);
        break;
    }
    case KAddressBookImportExportContactFields::HomeAddressLabel:
    {
        KContacts::Address address = contact.address(KContacts::Address::Home);
        address.setLabel(value);
        contact.insertAddress(address);
        break;
    }
    case KAddressBookImportExportContactFields::BusinessAddressStreet:
    {
        KContacts::Address address = contact.address(KContacts::Address::Work);
        address.setStreet(value);
        contact.insertAddress(address);
        break;
    }
    case KAddressBookImportExportContactFields::BusinessAddressPostOfficeBox:
    {
        KContacts::Address address = contact.address(KContacts::Address::Work);
        address.setPostOfficeBox(value);
        contact.insertAddress(address);
        break;
    }
    case KAddressBookImportExportContactFields::BusinessAddressLocality:
    {
        KContacts::Address address = contact.address(KContacts::Address::Work);
        address.setLocality(value);
        contact.insertAddress(address);
        break;
    }
    case KAddressBookImportExportContactFields::BusinessAddressRegion:
    {
        KContacts::Address address = contact.address(KContacts::Address::Work);
        address.setRegion(value);
        contact.insertAddress(address);
        break;
    }
    case KAddressBookImportExportContactFields::BusinessAddressPostalCode:
    {
        KContacts::Address address = contact.address(KContacts::Address::Work);
        address.setPostalCode(value);
        contact.insertAddress(address);
        break;
    }
    case KAddressBookImportExportContactFields::BusinessAddressCountry:
    {
        KContacts::Address address = contact.address(KContacts::Address::Work);
        address.setCountry(value);
        contact.insertAddress(address);
        break;
    }
    case KAddressBookImportExportContactFields::BusinessAddressLabel:
    {
        KContacts::Address address = contact.address(KContacts::Address::Work);
        address.setLabel(value);
        contact.insertAddress(address);
        break;
    }
    case BlogFeed:
        contact.insertCustom(QStringLiteral("KADDRESSBOOK"),
                             QStringLiteral("BlogFeed"), value);
        break;
    case Profession:
        contact.insertCustom(QStringLiteral("KADDRESSBOOK"),
                             QStringLiteral("X-Profession"), value);
        break;
    case Office:
        contact.insertCustom(QStringLiteral("KADDRESSBOOK"),
                             QStringLiteral("X-Office"), value);
        break;
    case Manager:
        contact.insertCustom(QStringLiteral("KADDRESSBOOK"),
                             QStringLiteral("X-ManagersName"), value);
        break;
    case Assistant:
        contact.insertCustom(QStringLiteral("KADDRESSBOOK"),
                             QStringLiteral("X-AssistantsName"), value);
        break;
    case Spouse:
        contact.insertCustom(QStringLiteral("KADDRESSBOOK"),
                             QStringLiteral("X-SpousesName"), value);
        break;
    }
}

QString KAddressBookImportExportContactFields::value(Field field, const KContacts::Addressee &contact)
{
    switch (field) {
    case Undefined:
        return QString();
    case FormattedName:
        return contact.formattedName();
    case Prefix:
        return contact.prefix();
    case GivenName:
        return contact.givenName();
    case AdditionalName:
        return contact.additionalName();
    case FamilyName:
        return contact.familyName();
    case Suffix:
        return contact.suffix();
    case NickName:
        return contact.nickName();
    case Birthday:
    {
        const QDateTime birthday = contact.birthday();
        if (birthday.date().isValid()) {
            return birthday.date().toString(Qt::ISODate);
        } else {
            return QString();
        }
    }
    case Anniversary:
        return contact.custom(QStringLiteral("KADDRESSBOOK"), QStringLiteral("X-Anniversary"));
    case HomeAddressStreet:
    {
        const KContacts::Address address = contact.address(KContacts::Address::Home);
        return address.street();
    }
    case HomeAddressPostOfficeBox:
    {
        const KContacts::Address address = contact.address(KContacts::Address::Home);
        return address.postOfficeBox();
    }
    case HomeAddressLocality:
    {
        const KContacts::Address address = contact.address(KContacts::Address::Home);
        return address.locality();
    }
    case HomeAddressRegion:
    {
        const KContacts::Address address = contact.address(KContacts::Address::Home);
        return address.region();
    }
    case HomeAddressPostalCode:
    {
        const KContacts::Address address = contact.address(KContacts::Address::Home);
        return address.postalCode();
    }
    case HomeAddressCountry:
    {
        const KContacts::Address address = contact.address(KContacts::Address::Home);
        return address.country();
    }
    case HomeAddressLabel:
    {
        const KContacts::Address address = contact.address(KContacts::Address::Home);
        return address.label();
    }
    case BusinessAddressStreet:
    {
        const KContacts::Address address = contact.address(KContacts::Address::Work);
        return address.street();
    }
    case BusinessAddressPostOfficeBox:
    {
        const KContacts::Address address = contact.address(KContacts::Address::Work);
        return address.postOfficeBox();
    }
    case BusinessAddressLocality:
    {
        const KContacts::Address address = contact.address(KContacts::Address::Work);
        return address.locality();
    }
    case BusinessAddressRegion:
    {
        const KContacts::Address address = contact.address(KContacts::Address::Work);
        return address.region();
    }
    case BusinessAddressPostalCode:
    {
        const KContacts::Address address = contact.address(KContacts::Address::Work);
        return address.postalCode();
    }
    case BusinessAddressCountry:
    {
        const KContacts::Address address = contact.address(KContacts::Address::Work);
        return address.country();
    }
    case BusinessAddressLabel:
    {
        const KContacts::Address address = contact.address(KContacts::Address::Work);
        return address.label();
    }
    case HomePhone:
        return contact.phoneNumber(KContacts::PhoneNumber::Home).number();
    case BusinessPhone:
        return contact.phoneNumber(KContacts::PhoneNumber::Work).number();
    case MobilePhone:
        return contact.phoneNumber(KContacts::PhoneNumber::Cell).number();
    case HomeFax:
        return contact.phoneNumber(KContacts::PhoneNumber::Home | KContacts::PhoneNumber::Fax).number();
    case BusinessFax:
        return contact.phoneNumber(KContacts::PhoneNumber::Work | KContacts::PhoneNumber::Fax).number();
    case CarPhone:
        return contact.phoneNumber(KContacts::PhoneNumber::Car).number();
    case Isdn:
        return contact.phoneNumber(KContacts::PhoneNumber::Isdn).number();
    case Pager:
        return contact.phoneNumber(KContacts::PhoneNumber::Pager).number();
    case PreferredEmail:
    {
        const QStringList emails = contact.emails();
        if (!emails.isEmpty()) {
            return emails.at(0);
        } else {
            return QString();
        }
    }
    case Email2:
    {
        const QStringList emails = contact.emails();
        if (emails.count() > 1) {
            return emails.at(1);
        } else {
            return QString();
        }
    }
    case Email3:
    {
        const QStringList emails = contact.emails();
        if (emails.count() > 2) {
            return emails.at(2);
        } else {
            return QString();
        }
    }
    case Email4:
    {
        const QStringList emails = contact.emails();
        if (emails.count() > 3) {
            return emails.at(3);
        } else {
            return QString();
        }
    }
    case Mailer:
        return contact.mailer();
    case Title:
        return contact.title();
    case Role:
        return contact.role();
    case Organization:
        return contact.organization();
    case Note:
        return contact.note();
    case Homepage:
        return contact.url().url().url();
    case BlogFeed:
        return contact.custom(QStringLiteral("KADDRESSBOOK"), QStringLiteral("BlogFeed"));
    case Profession:
        return contact.custom(QStringLiteral("KADDRESSBOOK"), QStringLiteral("X-Profession"));
    case Office:
        return contact.custom(QStringLiteral("KADDRESSBOOK"), QStringLiteral("X-Office"));
    case Manager:
        return contact.custom(QStringLiteral("KADDRESSBOOK"), QStringLiteral("X-ManagersName"));
    case Assistant:
        return contact.custom(QStringLiteral("KADDRESSBOOK"), QStringLiteral("X-AssistantsName"));
    case Spouse:
        return contact.custom(QStringLiteral("KADDRESSBOOK"), QStringLiteral("X-SpousesName"));
    }

    return QString();
}
