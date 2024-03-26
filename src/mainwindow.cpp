/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2007 Tobias Koenig <tokoe@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "mainwindow.h"
#include "mainwidget.h"
#include "settings.h"
#include <KAboutData>
#include <KActionCollection>
#include <KConfigGroup>
#include <KEditToolBar>
#include <KLocalizedString>
#include <KMessageBox>
#include <KSharedConfig>
#include <KShortcutsDialog>
#include <KStandardAction>
#include <KToggleFullScreenAction>
#include <KToolBar>
#include <QAction>
#include <QFontDatabase>
#include <QMenuBar>
#include <QPointer>
#include <QVBoxLayout>

// signal handler for SIGINT & SIGTERM
#ifdef Q_OS_UNIX
#include <KSignalHandler>
#include <signal.h>
#include <unistd.h>
#endif

#ifdef WITH_KUSERFEEDBACK
#include "userfeedback/userfeedbackmanager.h"
#include <KUserFeedback/NotificationPopup>
#include <KUserFeedback/Provider>
#endif

#include <PimCommon/NeedUpdateVersionUtils>
#include <PimCommon/NeedUpdateVersionWidget>

MainWindow::MainWindow()
    : KXmlGuiWindow(nullptr)
    , mMainWidget(new MainWidget(this, this))
{
#ifdef Q_OS_UNIX
    /**
     * Set up signal handler for SIGINT and SIGTERM
     */
    KSignalHandler::self()->watchSignal(SIGINT);
    KSignalHandler::self()->watchSignal(SIGTERM);
    connect(KSignalHandler::self(), &KSignalHandler::signalReceived, this, [this](int signal) {
        if (signal == SIGINT || signal == SIGTERM) {
            printf("Shutting down...\n");
            if (mMainWidget->canClose()) {
                close();
            }
        }
    });
#endif
    auto mainWidget = new QWidget(this);
    auto mainWidgetLayout = new QVBoxLayout(mainWidget);
    mainWidgetLayout->setContentsMargins({});
    mainWidgetLayout->setSpacing(0);
    if (PimCommon::NeedUpdateVersionUtils::checkVersion()) {
        const auto status = PimCommon::NeedUpdateVersionUtils::obsoleteVersionStatus(KAboutData::applicationData().version(), QDate::currentDate());
        if (status != PimCommon::NeedUpdateVersionUtils::ObsoleteVersion::NotObsoleteYet) {
            auto needUpdateVersionWidget = new PimCommon::NeedUpdateVersionWidget(this);
            mainWidgetLayout->addWidget(needUpdateVersionWidget);
            needUpdateVersionWidget->setObsoleteVersion(status);
        }
    }
    mainWidgetLayout->addWidget(mMainWidget);

    setCentralWidget(mainWidget);

    setStandardToolBarMenuEnabled(true);

    toolBar()->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    initActions();
    setupGUI(Save | Create, QStringLiteral("kaddressbookui.rc"));

    setAutoSaveSettings();

    mShowMenuBarAction->setChecked(Settings::self()->showMenuBar());
    slotToggleMenubar(true);
#ifdef WITH_KUSERFEEDBACK
    auto userFeedBackNotificationPopup = new KUserFeedback::NotificationPopup(this);
    userFeedBackNotificationPopup->setFeedbackProvider(UserFeedBackManager::self()->userFeedbackProvider());
#endif
}

MainWindow::~MainWindow() = default;

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
    if (menuBar()) {
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
    mShowFullScreenAction = KStandardAction::fullScreen(nullptr, nullptr, this, actionCollection());
    actionCollection()->setDefaultShortcut(mShowFullScreenAction, Qt::Key_F11);
    connect(mShowFullScreenAction, &QAction::toggled, this, &MainWindow::slotFullScreen);
}

void MainWindow::slotFullScreen(bool t)
{
    KToggleFullScreenAction::setFullScreen(this, t);
    QMenuBar *mb = menuBar();
    if (t) {
        auto b = new QToolButton(mb);
        b->setDefaultAction(mShowFullScreenAction);
        b->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Ignored));
        b->setFont(QFontDatabase::systemFont(QFontDatabase::SmallestReadableFont));
        mb->setCornerWidget(b, Qt::TopRightCorner);
        b->setVisible(true);
        b->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    } else {
        QWidget *w = mb->cornerWidget(Qt::TopRightCorner);
        if (w) {
            w->deleteLater();
        }
    }
}

void MainWindow::updateHamburgerMenu()
{
    delete mHamburgerMenu->menu();
    auto menu = new QMenu(this);
    menu->addAction(actionCollection()->action(QStringLiteral("akonadi_resource_create")));
    menu->addSeparator();

    menu->addAction(actionCollection()->action(KStandardAction::name(KStandardAction::Print)));
    menu->addAction(actionCollection()->action(KStandardAction::name(KStandardAction::PrintPreview)));
    menu->addSeparator();

    menu->addAction(actionCollection()->action(KStandardAction::name(KStandardAction::Quit)));
    mHamburgerMenu->setMenu(menu);
}

void MainWindow::configure()
{
    mMainWidget->configure();
}

void MainWindow::configureKeyBindings()
{
    auto dlg = new KShortcutsDialog(this);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->setModal(false);
    dlg->addCollection(actionCollection());

    connect(dlg, &KShortcutsDialog::saved, this, [this]() {
        mMainWidget->updateQuickSearchText();
    });

    dlg->configure(true /* save settings on accept*/);
}

void MainWindow::configureToolbars()
{
    KConfigGroup grp = KSharedConfig::openConfig()->group(QStringLiteral("MainWindow"));
    saveMainWindowSettings(grp);

    QPointer<KEditToolBar> dlg = new KEditToolBar(factory());
    connect(dlg.data(), &KEditToolBar::newToolBarConfig, this, &MainWindow::newToolbarConfig);
    dlg->exec();
    delete dlg;
}

void MainWindow::newToolbarConfig()
{
    createGUI(QStringLiteral("kaddressbookui.rc"));

    applyMainWindowSettings(KSharedConfig::openConfig()->group(QStringLiteral("MainWindow")));
}

void MainWindow::slotToggleMenubar(bool dontShowWarning)
{
    if (menuBar()) {
        if (mShowMenuBarAction->isChecked()) {
            menuBar()->show();
        } else {
            if (!dontShowWarning && (!toolBar()->isVisible() || !toolBar()->actions().contains(mHamburgerMenu))) {
                const QString accel = mShowMenuBarAction->shortcut().toString(QKeySequence::NativeText);
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

#include "moc_mainwindow.cpp"
