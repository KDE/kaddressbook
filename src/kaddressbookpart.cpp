/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2009-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kaddressbookpart.h"
#include <kcoreaddons_version.h>
#if KCOREADDONS_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include "kaddressbookmigrateapplication.h"
#endif
#include "mainwidget.h"

#include "kaddressbook_debug.h"
#include <KActionCollection>
#include <KLocalizedString>
#include <KPluginFactory>
#include <QAction>
#include <QIcon>

#include <QVBoxLayout>

K_PLUGIN_FACTORY(KAddressBookFactory, registerPlugin<KAddressBookPart>();)

KAddressBookPart::KAddressBookPart(QWidget *parentWidget, QObject *parent, const QVariantList &)
    : KParts::Part(parent)
{
    setComponentName(QStringLiteral("kaddressbook"), i18n("KAddressBook"));
#if KCOREADDONS_VERSION < QT_VERSION_CHECK(6, 0, 0)
    KAddressBookMigrateApplication migrate;
    migrate.migrate();
#endif

    // create a canvas to insert our widget
    auto canvas = new QWidget(parentWidget);
    canvas->setFocusPolicy(Qt::ClickFocus);
    setWidget(canvas);
    auto topLayout = new QVBoxLayout(canvas);

    mMainWidget = new MainWidget(this, canvas);
    initAction();

    topLayout->addWidget(mMainWidget);
    topLayout->setContentsMargins(0, 0, 0, 0);
    setXMLFile(QStringLiteral("kaddressbookui.rc"));
}

void KAddressBookPart::updateQuickSearchText()
{
    mMainWidget->updateQuickSearchText();
}

void KAddressBookPart::initAction()
{
    auto action = new QAction(QIcon::fromTheme(QStringLiteral("configure")), i18n("&Configure KAddressBook..."), this);
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

KAddressBookPart::~KAddressBookPart()
{
}

void KAddressBookPart::guiActivateEvent(KParts::GUIActivateEvent *e)
{
    KParts::Part::guiActivateEvent(e);
    mMainWidget->initializePluginActions();
}

#include "kaddressbookpart.moc"
