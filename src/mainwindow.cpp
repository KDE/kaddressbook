/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2007 Tobias Koenig <tokoe@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "mainwindow.h"
#include "mainwidget.h"
#include "settings.h"
#include <KActionCollection>
#include <KConfigGroup>
#include <KEditToolBar>
#include <KLocalizedString>
#include <KMessageBox>
#include <KSharedConfig>
#include <KShortcutsDialog>
#include <KStandardAction>
#include <KToolBar>
#include <QAction>
#include <QMenuBar>
#include <QPointer>
#ifdef WITH_KUSERFEEDBACK
#include "userfeedback/userfeedbackmanager.h"
#include <KUserFeedback/NotificationPopup>
#include <KUserFeedback/Provider>
#endif

MainWindow::MainWindow()
    : KXmlGuiWindow(nullptr)
{
    mMainWidget = new MainWidget(this, this);

    setCentralWidget(mMainWidget);

    initActions();

    setStandardToolBarMenuEnabled(true);

    toolBar()->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    setupGUI(Save | Create, QStringLiteral("kaddressbookui.rc"));

    setAutoSaveSettings();

    mShowMenuBarAction->setChecked(Settings::self()->showMenuBar());
    slotToggleMenubar(true);
#ifdef WITH_KUSERFEEDBACK
    auto userFeedBackNotificationPopup = new KUserFeedback::NotificationPopup(this);
    userFeedBackNotificationPopup->setFeedbackProvider(UserFeedBackManager::self()->userFeedbackProvider());
#endif
}

MainWindow::~MainWindow()
{
}

MainWidget *MainWindow::mainWidget() const
{
    return mMainWidget;
}

void MainWindow::initActions()
{
    KStandardAction::quit(this, &MainWindow::close, actionCollection());
    mShowMenuBarAction = KStandardAction::showMenubar(this, &MainWindow::slotToggleMenubar, actionCollection());

    QAction *action = KStandardAction::keyBindings(this, &MainWindow::configureKeyBindings, actionCollection());
    action->setWhatsThis(i18nc("@info:whatsthis",
                               "You will be presented with a dialog where you can configure "
                               "the application-wide shortcuts."));
    KStandardAction::configureToolbars(this, &MainWindow::configureToolbars, actionCollection());
    KStandardAction::preferences(this, &MainWindow::configure, actionCollection());
    mHamburgerMenu = KStandardAction::hamburgerMenu(nullptr, nullptr, actionCollection());
    mHamburgerMenu->setShowMenuBarAction(mShowMenuBarAction);
    mHamburgerMenu->setMenuBar(menuBar());
    connect(mHamburgerMenu, &KHamburgerMenu::aboutToShowMenu, this, [this]() {
        updateHamburgerMenu();
        // Immediately disconnect. We only need to run this once, but on demand.
        // NOTE: The nullptr at the end disconnects all connections between
        // q and mHamburgerMenu's aboutToShowMenu signal.
        disconnect(mHamburgerMenu, &KHamburgerMenu::aboutToShowMenu, this, nullptr);
    });
}

void MainWindow::updateHamburgerMenu()
{
    QMenu *menu = new QMenu;
    menu->addAction(actionCollection()->action(QStringLiteral("akonadi_contact_create")));
    menu->addAction(actionCollection()->action(QStringLiteral("akonadi_contact_group_create")));
    menu->addSeparator();
    menu->addAction(actionCollection()->action(QStringLiteral("akonadi_contact_item_edit")));
    menu->addSeparator();
    menu->addAction(actionCollection()->action(QLatin1String(KStandardAction::name(KStandardAction::Print))));
    menu->addSeparator();
    menu->addAction(actionCollection()->action(QLatin1String(KStandardAction::name(KStandardAction::Quit))));
    mHamburgerMenu->setMenu(menu);
}

void MainWindow::configure()
{
    mMainWidget->configure();
}

void MainWindow::configureKeyBindings()
{
    if (KShortcutsDialog::configure(actionCollection(), KShortcutsEditor::LetterShortcutsAllowed, this)) {
        mMainWidget->updateQuickSearchText();
    }
}

void MainWindow::configureToolbars()
{
    KConfigGroup grp = KSharedConfig::openConfig()->group("MainWindow");
    saveMainWindowSettings(grp);

    QPointer<KEditToolBar> dlg = new KEditToolBar(factory());
    connect(dlg.data(), &KEditToolBar::newToolBarConfig, this, &MainWindow::newToolbarConfig);
    dlg->exec();
    delete dlg;
}

void MainWindow::newToolbarConfig()
{
    createGUI(QStringLiteral("kaddressbookui.rc"));

    applyMainWindowSettings(KSharedConfig::openConfig()->group("MainWindow"));
}

void MainWindow::slotToggleMenubar(bool dontShowWarning)
{
    if (menuBar()) {
        if (mShowMenuBarAction->isChecked()) {
            menuBar()->show();
        } else {
            if (!dontShowWarning && (!toolBar()->isVisible() || !toolBar()->actions().contains(mHamburgerMenu))) {
                const QString accel = mShowMenuBarAction->shortcut().toString();
                KMessageBox::information(this,
                                         i18n("<qt>This will hide the menu bar completely."
                                              " You can show it again by typing %1.</qt>",
                                              accel),
                                         i18n("Hide menu bar"),
                                         QStringLiteral("HideMenuBarWarning"));
            }
            menuBar()->hide();
        }
        Settings::self()->setShowMenuBar(mShowMenuBarAction->isChecked());
    }
}
