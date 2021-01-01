/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2009-2021 Laurent Montel <montel@kde.org>

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
                 i18n("Copyright © 2007–2021 KAddressBook authors"))
{
    addAuthor(i18n("Laurent Montel"), i18n("Current maintainer"), QStringLiteral("montel@kde.org"));
    addAuthor(i18n("Tobias Koenig"), i18n("Previous maintainer"), QStringLiteral("tokoe@kde.org"));
}

AboutData::~AboutData()
{
}
