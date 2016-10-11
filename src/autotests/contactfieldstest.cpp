
#include "../kaddressbookimportexportcontactfields.h"

#include <qtest.h>

#include <QObject>

class ContactFieldsTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testFieldCount();
    void testSetGet();

private:
    KContacts::Addressee mContact;
};

QTEST_MAIN(ContactFieldsTest)

static const QString s_formattedName(QStringLiteral("User, Joe"));
static const QString s_prefix(QStringLiteral("Mr."));
static const QString s_givenName(QStringLiteral("Joe"));
static const QString s_additionalName(QStringLiteral("Doe"));
static const QString s_familyName(QStringLiteral("User"));
static const QString s_suffix(QStringLiteral("Jr."));
static const QString s_nickName(QStringLiteral("joe"));
static const QString s_birthday(QStringLiteral("1966-12-03"));
static const QString s_anniversary(QStringLiteral("1980-10-02"));
static const QString s_homeAddressStreet(QStringLiteral("My Home Street"));
static const QString s_homeAddressPostOfficeBox(QStringLiteral("My Home POB"));
static const QString s_homeAddressLocality(QStringLiteral("My Home Locality"));
static const QString s_homeAddressRegion(QStringLiteral("My Home Address"));
static const QString s_homeAddressPostalCode(QStringLiteral("My Home Postal Code"));
static const QString s_homeAddressCountry(QStringLiteral("My Home Country"));
static const QString s_homeAddressLabel(QStringLiteral("My Home Label"));
static const QString s_businessAddressStreet(QStringLiteral("My Business Street"));
static const QString s_businessAddressPostOfficeBox(QStringLiteral("My Business POB"));
static const QString s_businessAddressLocality(QStringLiteral("My Business Locality"));
static const QString s_businessAddressRegion(QStringLiteral("My Business Region"));
static const QString s_businessAddressPostalCode(QStringLiteral("My Business Postal Code"));
static const QString s_businessAddressCountry(QStringLiteral("My Business Country"));
static const QString s_businessAddressLabel(QStringLiteral("My Business Label"));
static const QString s_homePhone(QStringLiteral("000111222"));
static const QString s_businessPhone(QStringLiteral("333444555"));
static const QString s_mobilePhone(QStringLiteral("666777888"));
static const QString s_homeFax(QStringLiteral("999000111"));
static const QString s_businessFax(QStringLiteral("222333444"));
static const QString s_carPhone(QStringLiteral("555666777"));
static const QString s_isdn(QStringLiteral("888999000"));
static const QString s_pager(QStringLiteral("111222333"));
static const QString s_preferredEmail(QStringLiteral("me@somewhere.org"));
static const QString s_email2(QStringLiteral("you@somewhere.org"));
static const QString s_email3(QStringLiteral("she@somewhere.org"));
static const QString s_email4(QStringLiteral("it@somewhere.org"));
static const QString s_mailer(QStringLiteral("kmail2"));
static const QString s_title(QStringLiteral("Chief"));
static const QString s_role(QStringLiteral("Developer"));
static const QString s_organization(QStringLiteral("KDE"));
static const QString s_note(QStringLiteral("That's a small note"));
static const QString s_homepage(QStringLiteral("http://www.kde.de"));
static const QString s_blogFeed(QStringLiteral("http://planetkde.org"));
static const QString s_profession(QStringLiteral("Developer"));
static const QString s_office(QStringLiteral("Room 2443"));
static const QString s_manager(QStringLiteral("Hans"));
static const QString s_assistant(QStringLiteral("Hins"));
static const QString s_spouse(QStringLiteral("My Darling"));

void ContactFieldsTest::testFieldCount()
{
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::allFields().count(), 48);
}

void ContactFieldsTest::testSetGet()
{
    KContacts::Addressee contact;

    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::FormattedName, s_formattedName, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::Prefix, s_prefix, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::GivenName, s_givenName, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::AdditionalName, s_additionalName, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::FamilyName, s_familyName, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::Suffix, s_suffix, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::NickName, s_nickName, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::Birthday, s_birthday, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::Anniversary, s_anniversary, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::HomeAddressStreet, s_homeAddressStreet, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::HomeAddressPostOfficeBox, s_homeAddressPostOfficeBox, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::HomeAddressLocality, s_homeAddressLocality, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::HomeAddressRegion, s_homeAddressRegion, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::HomeAddressPostalCode, s_homeAddressPostalCode, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::HomeAddressCountry, s_homeAddressCountry, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::HomeAddressLabel, s_homeAddressLabel, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::BusinessAddressStreet, s_businessAddressStreet, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::BusinessAddressPostOfficeBox, s_businessAddressPostOfficeBox, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::BusinessAddressLocality, s_businessAddressLocality, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::BusinessAddressRegion, s_businessAddressRegion, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::BusinessAddressPostalCode, s_businessAddressPostalCode, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::BusinessAddressCountry, s_businessAddressCountry, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::BusinessAddressLabel, s_businessAddressLabel, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::HomePhone, s_homePhone, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::BusinessPhone, s_businessPhone, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::MobilePhone, s_mobilePhone, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::HomeFax, s_homeFax, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::BusinessFax, s_businessFax, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::CarPhone, s_carPhone, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::Isdn, s_isdn, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::Pager, s_pager, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::PreferredEmail, s_preferredEmail, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::Email2, s_email2, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::Email3, s_email3, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::Email4, s_email4, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::Mailer, s_mailer, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::Title, s_title, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::Role, s_role, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::Organization, s_organization, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::Note, s_note, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::Homepage, s_homepage, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::BlogFeed, s_blogFeed, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::Profession, s_profession, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::Office, s_office, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::Manager, s_manager, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::Assistant, s_assistant, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::Anniversary, s_anniversary, contact);
    KAddressBookImportExport::KAddressBookImportExportContactFields::setValue(KAddressBookImportExport::KAddressBookImportExportContactFields::Spouse, s_spouse, contact);

    const KContacts::Addressee contactCopy = contact;

    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::FormattedName, contactCopy), s_formattedName);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::Prefix, contactCopy), s_prefix);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::GivenName, contactCopy), s_givenName);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::AdditionalName, contactCopy), s_additionalName);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::FamilyName, contactCopy), s_familyName);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::Suffix, contactCopy), s_suffix);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::NickName, contactCopy), s_nickName);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::Birthday, contactCopy), s_birthday);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::Anniversary, contactCopy), s_anniversary);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::HomeAddressStreet, contactCopy), s_homeAddressStreet);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::HomeAddressPostOfficeBox, contactCopy), s_homeAddressPostOfficeBox);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::HomeAddressLocality, contactCopy), s_homeAddressLocality);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::HomeAddressRegion, contactCopy), s_homeAddressRegion);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::HomeAddressPostalCode, contactCopy), s_homeAddressPostalCode);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::HomeAddressCountry, contactCopy), s_homeAddressCountry);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::HomeAddressLabel, contactCopy), s_homeAddressLabel);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::BusinessAddressStreet, contactCopy), s_businessAddressStreet);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::BusinessAddressPostOfficeBox, contactCopy), s_businessAddressPostOfficeBox);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::BusinessAddressLocality, contactCopy), s_businessAddressLocality);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::BusinessAddressRegion, contactCopy), s_businessAddressRegion);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::BusinessAddressPostalCode, contactCopy), s_businessAddressPostalCode);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::BusinessAddressCountry, contactCopy), s_businessAddressCountry);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::BusinessAddressLabel, contactCopy), s_businessAddressLabel);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::HomePhone, contactCopy), s_homePhone);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::BusinessPhone, contactCopy), s_businessPhone);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::MobilePhone, contactCopy), s_mobilePhone);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::HomeFax, contactCopy), s_homeFax);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::BusinessFax, contactCopy), s_businessFax);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::CarPhone, contactCopy), s_carPhone);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::Isdn, contactCopy), s_isdn);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::Pager, contactCopy), s_pager);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::PreferredEmail, contactCopy), s_preferredEmail);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::Email2, contactCopy), s_email2);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::Email3, contactCopy), s_email3);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::Email4, contactCopy), s_email4);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::Mailer, contactCopy), s_mailer);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::Title, contactCopy), s_title);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::Role, contactCopy), s_role);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::Organization, contactCopy), s_organization);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::Note, contactCopy), s_note);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::Homepage, contactCopy), s_homepage);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::BlogFeed, contactCopy), s_blogFeed);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::Profession, contactCopy), s_profession);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::Office, contactCopy), s_office);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::Manager, contactCopy), s_manager);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::Assistant, contactCopy), s_assistant);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::Anniversary, contactCopy), s_anniversary);
    QCOMPARE(KAddressBookImportExport::KAddressBookImportExportContactFields::value(KAddressBookImportExport::KAddressBookImportExportContactFields::Spouse, contactCopy), s_spouse);
}

#include "contactfieldstest.moc"
