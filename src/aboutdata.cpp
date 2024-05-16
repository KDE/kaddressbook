/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2009-2024 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "aboutdata.h"

#include "kaddressbook-version.h"

#include <KLocalizedString>

AboutData::AboutData()
    : KAboutData(QStringLiteral("kaddressbook"),
                 i18n("KAddressBook"),
                 QStringLiteral(KADDRESSBOOK_VERSION),
                 i18n("The KDE Address Book Application"),
                 KAboutLicense::GPL_V2,
                 i18n("Copyright © 2007–%1 KAddressBook authors", QStringLiteral("2024")))
{
    addAuthor(i18nc("@info:credit", "Laurent Montel"), i18n("Current maintainer"), QStringLiteral("montel@kde.org"));
    addAuthor(i18nc("@info:credit", "Tobias König"), i18n("Previous maintainer"), QStringLiteral("tokoe@kde.org"));
    addAuthor(i18nc("@info:credit", "Stefan Taferner"), i18n("Original Author (1998)"), QStringLiteral("taferner@kde.org"));
    addAuthor(i18nc("@info:credit", "David Bryant"),
              i18n("Documentation (2019)"),
              QStringLiteral("davidbryant@gvtc.com"),
              QStringLiteral("https://davidcbryant.net"));
}

AboutData::~AboutData() = default;
