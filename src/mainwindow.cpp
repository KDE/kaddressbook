/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2007 Tobias Koenig <tokoe@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "mainwindow.h"
using namespace Qt::Literals::StringLiterals;

#include "config-kaddressbook.h"
#include "mainwidget.h"
#include "settings.h"
#include "whatsnew/whatsnewtranslations.h"
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
#include <csignal>
#include <unistd.h>
#endif

#if KADDRESSBOOK_WITH_KUSERFEEDBACK
#include "userfeedback/userfeedbackmanager.h"
#include <KUserFeedback/NotificationPopup>
#include <KUserFeedback/Provider>
#endif

#if HAVE_TEXTUTILS_HAS_WHATSNEW_SUPPORT
#include <TextAddonsWidgets/NeedUpdateVersionUtils>
#include <TextAddonsWidgets/NeedUpdateVersionWidget>
#include <TextAddonsWidgets/WhatsNewMessageWidget>
#else
#include <PimCommon/NeedUpdateVersionUtils>
#include <PimCommon/NeedUpdateVersionWidget>
#include <PimCommon/WhatsNewMessageWidget>
#endif

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
#if HAVE_TEXTUTILS_HAS_WHATSNEW_SUPPORT
    if (TextAddonsWidgets::NeedUpdateVersionUtils::checkVersion()) {
        const auto status =
            TextAddonsWidgets::NeedUpdateVersionUtils::obsoleteVersionStatus(QLatin1String(KADDRESSBOOK_RELEASE_VERSION_DATE), QDate::currentDate());
        if (status != TextAddonsWidgets::NeedUpdateVersionUtils::ObsoleteVersion::NotObsoleteYet) {
            auto needUpdateVersionWidget = new TextAddonsWidgets::NeedUpdateVersionWidget(this);
            mainWidgetLayout->addWidget(needUpdateVersionWidget);
            needUpdateVersionWidget->setObsoleteVersion(status);
        }
    }
#else
    if (PimCommon::NeedUpdateVersionUtils::checkVersion()) {
        const auto status = PimCommon::NeedUpdateVersionUtils::obsoleteVersionStatus(QLatin1String(KADDRESSBOOK_RELEASE_VERSION_DATE), QDate::currentDate());
        if (status != PimCommon::NeedUpdateVersionUtils::ObsoleteVersion::NotObsoleteYet) {
            auto needUpdateVersionWidget = new PimCommon::NeedUpdateVersionWidget(this);
            mainWidgetLayout->addWidget(needUpdateVersionWidget);
            needUpdateVersionWidget->setObsoleteVersion(status);
        }
    }
#endif
    WhatsNewTranslations translations;
    const QString newFeaturesMD5 = translations.newFeaturesMD5();
    if (!newFeaturesMD5.isEmpty()) {
        const QString previousNewFeaturesMD5 = Settings::self()->previousNewFeaturesMD5();
        if (!previousNewFeaturesMD5.isEmpty()) {
            const bool hasNewFeature = (previousNewFeaturesMD5 != newFeaturesMD5);
            if (hasNewFeature) {
#if HAVE_TEXTUTILS_HAS_WHATSNEW_SUPPORT
                auto whatsNewMessageWidget = new TextAddonsWidgets::WhatsNewMessageWidget(this, i18n("KAddressBook"));
#else
                auto whatsNewMessageWidget = new PimCommon::WhatsNewMessageWidget(this, i18n("KAddressBook"));
#endif
                whatsNewMessageWidget->setWhatsNewInfos(translations.createWhatsNewInfo());
                whatsNewMessageWidget->setObjectName(u"whatsNewMessageWidget"_s);
                mainWidgetLayout->addWidget(whatsNewMessageWidget);
                Settings::self()->setPreviousNewFeaturesMD5(newFeaturesMD5);
                whatsNewMessageWidget->animatedShow();
            }
        } else {
            Settings::self()->setPreviousNewFeaturesMD5(newFeaturesMD5);
        }
    }

    mainWidgetLayout->addWidget(mMainWidget);
    setCentralWidget(mainWidget);

    setStandardToolBarMenuEnabled(true);

    toolBar()->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    initActions();
    setupGUI(Save | Create, u"kaddressbookui.rc"_s);

    setAutoSaveSettings();

    mShowMenuBarAction->setChecked(Settings::self()->showMenuBar());
    slotToggleMenubar(true);
#if KADDRESSBOOK_WITH_KUSERFEEDBACK
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
    KStandardActions::quit(this, &MainWindow::close, actionCollection());
    mShowMenuBarAction = KStandardAction::showMenubar(this, &MainWindow::slotToggleMenubar, actionCollection());

    QAction *action = KStandardActions::keyBindings(this, &MainWindow::configureKeyBindings, actionCollection());
    action->setWhatsThis(i18nc("@info:whatsthis",
                               "You will be presented with a dialog where you can configure "
                               "the application-wide shortcuts."));
    KStandardActions::configureToolbars(this, &MainWindow::configureToolbars, actionCollection());
    KStandardActions::preferences(this, &MainWindow::configure, actionCollection());
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
    menu->addAction(actionCollection()->action(u"akonadi_resource_create"_s));
    menu->addSeparator();

    menu->addAction(actionCollection()->action(KStandardAction::name(KStandardAction::Print)));
    menu->addAction(actionCollection()->action(KStandardAction::name(KStandardAction::PrintPreview)));
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
    KConfigGroup grp = KSharedConfig::openConfig()->group(u"MainWindow"_s);
    saveMainWindowSettings(grp);

    QPointer<KEditToolBar> dlg = new KEditToolBar(factory());
    connect(dlg.data(), &KEditToolBar::newToolBarConfig, this, &MainWindow::newToolbarConfig);
    dlg->exec();
    delete dlg;
}

void MainWindow::newToolbarConfig()
{
    createGUI(u"kaddressbookui.rc"_s);

    applyMainWindowSettings(KSharedConfig::openConfig()->group(u"MainWindow"_s));
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
                                         i18nc("@title:window", "Hide menu bar"),
                                         u"HideMenuBarWarning"_s);
            }
            menuBar()->hide();
        }
        Settings::self()->setShowMenuBar(mShowMenuBarAction->isChecked());
        Settings::self()->save();
    }
}

#include "moc_mainwindow.cpp"
