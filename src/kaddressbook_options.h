/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <KLocalizedString>
#include <QCommandLineOption>
#include <QCommandLineParser>

static void kaddressbook_options(QCommandLineParser *parser)
{
    parser->addOption(QCommandLineOption(QStringLiteral("import"), i18n("Import the given file")));

    parser->addOption(QCommandLineOption(QStringLiteral("newcontact"), i18n("Open the 'New Contact' dialog")));

    parser->addOption(QCommandLineOption(QStringLiteral("view"), i18n("Display the specified contact (by URL)"), QStringLiteral("url")));

    parser->addPositionalArgument(QStringLiteral("urls"), i18n("Files or URLs. The user will be asked whether to import."), QStringLiteral("[urls...]"));

#ifdef WITH_KUSERFEEDBACK
    parser->addOption(QCommandLineOption(QStringLiteral("feedback"), i18n("Lists the available options for user feedback")));
#endif
}

