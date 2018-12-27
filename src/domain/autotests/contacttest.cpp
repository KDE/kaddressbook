/*
  This file is part of KAddressBook.

  Copyright (C) 2016-2019 Laurent Montel <montel@kde.org>

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

#include "contacttest.h"
#include "../contact.h"
#include <QTest>

ContactTest::ContactTest(QObject *parent)
    : QObject(parent)
{
}

ContactTest::~ContactTest()
{
}

void ContactTest::shouldHaveDefaultValue()
{
    Domain::Contact contact;
    QVERIFY(contact.addressee().isEmpty());
}

void ContactTest::shouldAssignAddresse()
{
    Domain::Contact contact;
    KContacts::Addressee address;
    address.setEmails(QStringList() << QStringLiteral("foo@foo.foo") << QStringLiteral("foo@foo.foo2"));
    contact.setAddressee(address);

    QCOMPARE(contact.addressee(), address);
}

QTEST_MAIN(ContactTest)
