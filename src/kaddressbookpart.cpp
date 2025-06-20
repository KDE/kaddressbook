/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2009-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kaddressbookpart.h"
#include "mainwidget.h"
#include "settings.h"
#include "whatsnew/whatsnewtranslations.h"

#include "kaddressbook_debug.h"

#include <PimCommon/WhatsNewMessageWidget>

#include <KActionCollection>
#include <KLocalizedString>
#include <KPluginFactory>
#include <QAction>
#include <QIcon>
#include <QVBoxLayout>

K_PLUGIN_FACTORY(KAddressBookFactory, registerPlugin<KAddressBookPart>();)

KAddressBookPart::KAddressBookPart(QWidget *parentWidget, QObject *parent, const KPluginMetaData &data, const QVariantList &)
    : KParts::Part(parent, data)
{
    setComponentName(QStringLiteral("kaddressbook"), i18n("KAddressBook"));

    // create a canvas to insert our widget
    auto canvas = new QWidget(parentWidget);
    canvas->setFocusPolicy(Qt::ClickFocus);
    setWidget(canvas);
    auto topLayout = new QVBoxLayout(canvas);

    const WhatsNewTranslations translations;
    const QString newFeaturesMD5 = translations.newFeaturesMD5();
    if (!newFeaturesMD5.isEmpty()) {
        const bool hasNewFeature = (Settings::self()->previousNewFeaturesMD5() != newFeaturesMD5);
        if (hasNewFeature) {
            auto whatsNewMessageWidget = new PimCommon::WhatsNewMessageWidget(parentWidget, i18n("KAddressBook"));
            whatsNewMessageWidget->setWhatsNewInfos(translations.createWhatsNewInfo());
            whatsNewMessageWidget->setObjectName(QStringLiteral("whatsNewMessageWidget"));
            topLayout->addWidget(whatsNewMessageWidget);
            Settings::self()->setPreviousNewFeaturesMD5(newFeaturesMD5);
            whatsNewMessageWidget->animatedShow();
        }
    }

    mMainWidget = new MainWidget(this, canvas);
    initAction();

    topLayout->addWidget(mMainWidget);
    topLayout->setContentsMargins({});

    setXMLFile(QStringLiteral("kaddressbookui_part.rc"));
}

void KAddressBookPart::updateQuickSearchText()
{
    mMainWidget->updateQuickSearchText();
}

void KAddressBookPart::initAction()
{
    auto action = new QAction(QIcon::fromTheme(QStringLiteral("configure")), i18n("&Configure KAddressBook…"), this);
    actionCollection()->addAction(QStringLiteral("kaddressbook_configure"), action);
    connect(action, &QAction::triggered, mMainWidget, &MainWidget::configure);
}

void KAddressBookPart::newContact()
{
    mMainWidget->newContact();
}

void KAddressBookPart::newGroup()
{
    mMainWidget->newGroup();
}

KAddressBookPart::~KAddressBookPart() = default;

void KAddressBookPart::guiActivateEvent(KParts::GUIActivateEvent *e)
{
    KParts::Part::guiActivateEvent(e);
    mMainWidget->initializePluginActions();
}

#include "kaddressbookpart.moc"

#include "moc_kaddressbookpart.cpp"
