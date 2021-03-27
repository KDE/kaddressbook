/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2009 Tobias Koenig <tokoe@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "kaddressbook_importexport_export.h"
#include <KContacts/Addressee>
namespace KAddressBookImportExport
{
/** Contact fields for import/export. */
class KADDRESSBOOK_IMPORTEXPORT_EXPORT ContactFields
{
public:
    /**
     * Describes the standard fields that are available for every contact
     */
    enum Field {
        Undefined = 0,

        FormattedName,
        Prefix,
        GivenName,
        AdditionalName,
        FamilyName,
        Suffix,
        NickName,

        Birthday,
        Anniversary,

        HomeAddressStreet,
        HomeAddressPostOfficeBox,
        HomeAddressLocality,
        HomeAddressRegion,
        HomeAddressPostalCode,
        HomeAddressCountry,
        HomeAddressLabel,

        BusinessAddressStreet,
        BusinessAddressPostOfficeBox,
        BusinessAddressLocality,
        BusinessAddressRegion,
        BusinessAddressPostalCode,
        BusinessAddressCountry,
        BusinessAddressLabel,

        HomePhone,
        BusinessPhone,
        MobilePhone,
        HomeFax,
        BusinessFax,
        CarPhone,
        Isdn,
        Pager,

        PreferredEmail,
        Email2,
        Email3,
        Email4,

        Mailer,
        Title,
        Role,
        Organization,
        Note,
        Homepage,

        BlogFeed,
        Profession,
        Office,
        Manager,
        Assistant,
        Spouse
    };

    /**
     * Defines a list of Field enums.
     */
    using Fields = QVector<Field>;

    /**
     * Returns the i18n label for the @p field.
     */
    Q_REQUIRED_RESULT static QString label(Field field);

    /**
     * Returns a list of all available fields.
     */
    Q_REQUIRED_RESULT static Fields allFields();

    /**
     * Sets the @p value of the @p field for the @p contact.
     */
    static void setValue(Field field, const QString &value, KContacts::Addressee &contact);

    /**
     * Returns the value for the @p field of the @p contact.
     */
    Q_REQUIRED_RESULT static QString value(Field field, const KContacts::Addressee &contact);
};
}

