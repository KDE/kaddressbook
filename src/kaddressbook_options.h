/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "config-kaddressbook.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>
#include <QCommandLineOption>
#include <QCommandLineParser>

static void kaddressbook_options(QCommandLineParser *parser)
{
    parser->addOption(QCommandLineOption(u"import"_s, i18nc("@info:shell", "Import the given file")));

    parser->addOption(QCommandLineOption(u"newcontact"_s, i18nc("@info:shell", "Open the 'New Contact' dialog")));

    parser->addOption(QCommandLineOption(u"view"_s, i18nc("@info:shell", "Display the specified contact (by URL)"), u"url"_s));

    parser->addPositionalArgument(u"urls"_s, i18nc("@info:shell", "Files or URLs. The user will be asked whether to import."), u"[urls...]"_s);

#if KADDRESSBOOK_WITH_KUSERFEEDBACK
    parser->addOption(QCommandLineOption(u"feedback"_s, i18nc("@info:shell", "Lists the available options for user feedback")));
#endif
}
