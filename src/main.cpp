/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2007 Tobias Koenig <tokoe@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "aboutdata.h"
#include "kaddressbook_options.h"
#include "mainwidget.h"
#include "mainwindow.h"

#include <KCrash>
#include <QCommandLineParser>

#include "kaddressbook_debug.h"
#include <kontactinterface/pimuniqueapplication.h>
#ifdef WITH_KUSERFEEDBACK
#include "userfeedback/kaddressbookuserfeedbackprovider.h"
#endif

#define HAVE_KICONTHEME __has_include(<KIconTheme>)
#if HAVE_KICONTHEME
#include <KIconTheme>
#endif

#define HAVE_STYLE_MANAGER __has_include(<KStyleManager>)
#if HAVE_STYLE_MANAGER
#include <KStyleManager>
#endif

//-----------------------------------------------------------------------------

class KAddressBookApplication : public KontactInterface::PimUniqueApplication
{
public:
    KAddressBookApplication(int &argc, char **argv[])
        : KontactInterface::PimUniqueApplication(argc, argv)
    {
    }

    int activate(const QStringList &arguments, const QString &) override;

private:
    MainWindow *mMainWindow = nullptr;
};

int KAddressBookApplication::activate(const QStringList &arguments, const QString &)
{
    if (!mMainWindow) {
        mMainWindow = new MainWindow;
        mMainWindow->show();
    }
    mMainWindow->mainWidget()->handleCommandLine(arguments);
    return 0;
}

int main(int argc, char **argv)
{
#if HAVE_KICONTHEME
    KIconTheme::initTheme();
#endif
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    KAddressBookApplication app(argc, &argv);
    app.setDesktopFileName(QStringLiteral("org.kde.kaddressbook"));
    KCrash::initialize();
#if HAVE_STYLE_MANAGER
    KStyleManager::initStyle();
#else // !HAVE_STYLE_MANAGER
#if defined(Q_OS_MACOS) || defined(Q_OS_WIN)
    QApplication::setStyle(QStringLiteral("breeze"));
#endif // defined(Q_OS_MACOS) || defined(Q_OS_WIN)
#endif // HAVE_STYLE_MANAGER
    KLocalizedString::setApplicationDomain(QByteArrayLiteral("kaddressbook"));

    AboutData about;
    app.setAboutData(about);
    app.setWindowIcon(QIcon::fromTheme(QStringLiteral("kaddressbook")));
    QCommandLineParser *cmdArgs = app.cmdArgs();
    kaddressbook_options(cmdArgs);

    const QStringList args = QCoreApplication::arguments();
    cmdArgs->process(args);
    about.processCommandLine(cmdArgs);

#ifdef WITH_KUSERFEEDBACK
    if (cmdArgs->isSet(QStringLiteral("feedback"))) {
        auto userFeedBack = new KAddressBookUserFeedbackProvider(nullptr);
        QTextStream(stdout) << userFeedBack->describeDataSources() << '\n';
        delete userFeedBack;
        return 0;
    }
#endif

    if (!KAddressBookApplication::start(args)) {
        qCWarning(KADDRESSBOOK_LOG) << "kaddressbook is already running, exiting.";
        return 0;
    }
    return app.exec();
}
