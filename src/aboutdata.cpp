/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2009-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "aboutdata.h"
using namespace Qt::Literals::StringLiterals;

#include "kaddressbook-version.h"

#include <KLocalizedString>

AboutData::AboutData()
    : KAboutData(u"kaddressbook"_s,
                 i18n("KAddressBook"),
                 QStringLiteral(KADDRESSBOOK_VERSION),
                 i18n("The KDE Address Book Application"),
                 KAboutLicense::GPL_V2,
                 i18n("Copyright © 2007–%1 KAddressBook authors", u"2026"_s))
{
    addAuthor(i18nc("@info:credit", "Laurent Montel"), i18n("Current maintainer"), u"montel@kde.org"_s);
    addAuthor(i18nc("@info:credit", "Tobias König"), i18n("Previous maintainer"), u"tokoe@kde.org"_s);
    addAuthor(i18nc("@info:credit", "Stefan Taferner"), i18n("Original Author (1998)"), u"taferner@kde.org"_s);
    addAuthor(i18nc("@info:credit", "David Bryant"), i18n("Documentation (2019)"), u"davidbryant@gvtc.com"_s, u"https://davidcbryant.net"_s);
}

AboutData::~AboutData() = default;
