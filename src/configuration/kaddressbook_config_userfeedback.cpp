/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kaddressbook_config_userfeedback.h"
#include <KAboutData>
#include <KLocalizedString>
#include <KPluginFactory>
#include <QHBoxLayout>
#ifdef WITH_KUSERFEEDBACK
#include "userfeedback/userfeedbackmanager.h"
#include <KUserFeedbackQt6/FeedbackConfigWidget>
#endif

using namespace KAddressBook;

K_PLUGIN_CLASS_WITH_JSON(KCMKaddressbookUserFeedBackConfig, "kaddressbook_config_userfeedback.json")

KCMKaddressbookUserFeedBackConfig::KCMKaddressbookUserFeedBackConfig(QObject *parent, const KPluginMetaData &data, const QVariantList &args)
    : KCModule(parent, data, args)
    , mUserFeedbackWidget(new KUserFeedback::FeedbackConfigWidget(widget()))
{
    auto lay = new QHBoxLayout(widget());
    lay->setContentsMargins({});
    connect(mUserFeedbackWidget, &KUserFeedback::FeedbackConfigWidget::configurationChanged, this, &KCMKaddressbookUserFeedBackConfig::markAsChanged);

    lay->addWidget(mUserFeedbackWidget);
    mUserFeedbackWidget->setFeedbackProvider(UserFeedBackManager::self()->userFeedbackProvider());
}

void KCMKaddressbookUserFeedBackConfig::save()
{
    UserFeedBackManager::self()->userFeedbackProvider()->setTelemetryMode(mUserFeedbackWidget->telemetryMode());
    UserFeedBackManager::self()->userFeedbackProvider()->setSurveyInterval(mUserFeedbackWidget->surveyInterval());
}

void KCMKaddressbookUserFeedBackConfig::load()
{
    mUserFeedbackWidget->setFeedbackProvider(UserFeedBackManager::self()->userFeedbackProvider());
}

void KCMKaddressbookUserFeedBackConfig::defaults()
{
    // FIXME ?
    mUserFeedbackWidget->setFeedbackProvider(UserFeedBackManager::self()->userFeedbackProvider());
}

#include "kaddressbook_config_userfeedback.moc"
