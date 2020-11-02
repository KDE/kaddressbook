/*
   SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kaddressbook_config_userfeedback.h"
#include <KAboutData>
#include <KPluginFactory>
#include <KLocalizedString>
#include <QHBoxLayout>
#ifdef WITH_KUSERFEEDBACK
#include <KUserFeedback/FeedbackConfigWidget>
#include "userfeedback/userfeedbackmanager.h"
#endif

using namespace KAddressBook;

K_PLUGIN_FACTORY(KCMKaddressbookUserFeedBackConfigFactory, registerPlugin<KCMKaddressbookUserFeedBackConfig>();
                 )

KCMKaddressbookUserFeedBackConfig::KCMKaddressbookUserFeedBackConfig(QWidget *parent, const QVariantList &args)
    : KCModule(parent, args)
{
    auto *lay = new QHBoxLayout(this);
    lay->setContentsMargins(0, 0, 0, 0);

    KAboutData *about = new KAboutData(QStringLiteral("kcmaddressbookuserfeedbackconfig"),
                                       i18n("Configure User FeedBack"),
                                       QString(), QString(), KAboutLicense::GPL,
                                       i18n("(c), 2020 Laurent Montel"));

    about->addAuthor(i18n("Laurent Montel"), QString(), QStringLiteral("montel@kde.org"));

    setAboutData(about);
    mUserFeedbackWidget = new KUserFeedback::FeedbackConfigWidget(this);
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
    //FIXME ?
    mUserFeedbackWidget->setFeedbackProvider(UserFeedBackManager::self()->userFeedbackProvider());
}

#include "kaddressbook_config_userfeedback.moc"
