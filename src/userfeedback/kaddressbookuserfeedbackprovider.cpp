/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kaddressbookuserfeedbackprovider.h"
using namespace Qt::Literals::StringLiterals;

#include <KUserFeedback/ApplicationVersionSource>
#include <KUserFeedback/LocaleInfoSource>
#include <KUserFeedback/PlatformInfoSource>
#include <KUserFeedback/QtVersionSource>
#include <KUserFeedback/ScreenInfoSource>
#include <KUserFeedback/StartCountSource>
#include <KUserFeedback/UsageTimeSource>

KAddressBookUserFeedbackProvider::KAddressBookUserFeedbackProvider(QObject *parent)
    : KUserFeedback::Provider(parent)
{
    setProductIdentifier(u"org.kde.kaddressbook"_s);
    setFeedbackServer(QUrl(u"https://telemetry.kde.org/"_s));
    setSubmissionInterval(7);
    setApplicationStartsUntilEncouragement(5);
    setEncouragementDelay(30);

    addDataSource(new KUserFeedback::ApplicationVersionSource);
    addDataSource(new KUserFeedback::PlatformInfoSource);
    addDataSource(new KUserFeedback::ScreenInfoSource);
    addDataSource(new KUserFeedback::QtVersionSource);

    addDataSource(new KUserFeedback::StartCountSource);
    addDataSource(new KUserFeedback::UsageTimeSource);

    addDataSource(new KUserFeedback::LocaleInfoSource);
}

KAddressBookUserFeedbackProvider::~KAddressBookUserFeedbackProvider() = default;

#include "moc_kaddressbookuserfeedbackprovider.cpp"
