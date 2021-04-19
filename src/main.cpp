/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2007 Tobias Koenig <tokoe@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "aboutdata.h"
#include "kaddressbook_options.h"
#include <kcoreaddons_version.h>
#if KCOREADDONS_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include "kaddressbookmigrateapplication.h"
#endif
#include "mainwidget.h"
#include "mainwindow.h"

#include <KCrash>
#include <QCommandLineParser>

#include "kaddressbook_debug.h"
#include <kontactinterface/pimuniqueapplication.h>
#ifdef WITH_KUSERFEEDBACK
#include "userfeedback/kaddressbookuserfeedbackprovider.h"
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
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
    KAddressBookApplication app(argc, &argv);
    app.setDesktopFileName(QStringLiteral("org.kde.kaddressbook"));
    KCrash::initialize();
    KLocalizedString::setApplicationDomain("kaddressbook");

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
#if KCOREADDONS_VERSION < QT_VERSION_CHECK(6, 0, 0)
    KAddressBookMigrateApplication migrate;
    migrate.migrate();
#endif
    return app.exec();
}
