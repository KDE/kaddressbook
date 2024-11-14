/*
   SPDX-FileCopyrightText: 2013-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "config-kaddressbook.h"
#include <KLocalizedString>
#include <QCommandLineOption>
#include <QCommandLineParser>

static void kaddressbook_options(QCommandLineParser *parser)
{
    parser->addOption(QCommandLineOption(QStringLiteral("import"), i18nc("@info:shell", "Import the given file")));

    parser->addOption(QCommandLineOption(QStringLiteral("newcontact"), i18nc("@info:shell", "Open the 'New Contact' dialog")));

    parser->addOption(QCommandLineOption(QStringLiteral("view"), i18nc("@info:shell", "Display the specified contact (by URL)"), QStringLiteral("url")));

    parser->addPositionalArgument(QStringLiteral("urls"),
                                  i18nc("@info:shell", "Files or URLs. The user will be asked whether to import."),
                                  QStringLiteral("[urls...]"));

#if KADDRESSBOOK_WITH_KUSERFEEDBACK
    parser->addOption(QCommandLineOption(QStringLiteral("feedback"), i18nc("@info:shell", "Lists the available options for user feedback")));
#endif
}
