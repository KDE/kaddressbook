/*
   Copyright (C) 2013-2020 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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
