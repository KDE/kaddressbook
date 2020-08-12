/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KADDRESSBOOK_OPTIONS_H
#define KADDRESSBOOK_OPTIONS_H

#include <QCommandLineParser>
#include <QCommandLineOption>
#include <KLocalizedString>

static void kaddressbook_options(QCommandLineParser *parser)
{
    parser->addOption(
        QCommandLineOption(
            QStringLiteral("import"),
            i18n("Import the given file")));

    parser->addOption(
        QCommandLineOption(
            QStringLiteral("newcontact"),
            i18n("Open the 'New Contact' dialog")));

    parser->addOption(
        QCommandLineOption(
            QStringLiteral("view"),
            i18n("Display the specified contact (by URL)"), QStringLiteral("url")));

    parser->addPositionalArgument(
        QStringLiteral("urls"),
        i18n("Files or URLs. The user will be asked whether to import."),
        QStringLiteral("[urls...]"));

#ifdef WITH_KUSERFEEDBACK
    parser->addOption(QCommandLineOption(QStringLiteral("feedback"), i18n("Lists the available options for user feedback")));
#endif
}

#endif // KADDRESSBOOK_OPTIONS_H
